const chokidar = require('chokidar');
const fs = require('fs');
const path = require('path');
// 如果你的 inquirer 版本较低，可能不需要 .default；如果报错请尝试去掉 .default
const inquirer = require('inquirer').default || require('inquirer');
const notifier = require('node-notifier');

const WATCH_DIR = 'files'; 
const DATA_FILE = './filedata.js';
const PUBLIC_FILE = './public/filedata.js';

let isProcessing = false;
const eventQueue = [];

// 读取逻辑：解析 window.FILE_DB
function readData() {
    if (!fs.existsSync(DATA_FILE)) return {};
    try {
        const content = fs.readFileSync(DATA_FILE, 'utf-8').trim();
        // 去掉 window.FILE_DB = 和结尾的分号
        const jsonStr = content.replace(/^window\.FILE_DB\s*=\s*/, '').replace(/;$/, '');
        // 使用 new Function 解析 JS 对象字面量
        return new Function(`return ${jsonStr}`)();
    } catch (e) {
        console.error("读取数据出错，请检查 filedata.js 格式");
        return null;
    }
}

// 写入逻辑
function saveData(data) {
    // 1. 先转成标准 JSON 字符串
    let jsonString = JSON.stringify(data, null, 4);
    
    // 2. 正则黑科技：把符合 JS 变量命名规范的键名引号去掉
    // 比如 "type": -> type: 
    // 但是 "1秋": 会保留，因为以数字开头不符合变量命名规范
    jsonString = jsonString.replace(/"([a-zA-Z_$][\w$]*)":/g, '$1:');

    // 3. 拼装回 window.FILE_DB 格式
    const output = `window.FILE_DB = ${jsonString};`;
    fs.writeFileSync(DATA_FILE, output, 'utf-8');
}

// 检查路径是否存在于树中
function existsInTree(tree, pathParts) {
    let current = tree;
    for (let i = 0; i < pathParts.length; i++) {
        const part = pathParts[i];
        if (!current || !current[part]) return false;
        
        // 如果不是路径的最后一部分，则需要进入 children 继续找
        if (i < pathParts.length - 1) {
            // 只有存在 children 且该层级是文件夹时才继续
            if (current[part].type === "folder" && current[part].children) {
                current = current[part].children;
            } else {
                return false; // 路径不匹配
            }
        }
    }
    return true;
}

// 同步文件到 public 目录的逻辑
function syncToPublic() {
    try {
        const publicDir = path.dirname(PUBLIC_FILE);
        // 如果 public 文件夹不存在，则递归创建
        if (!fs.existsSync(publicDir)) {
            fs.mkdirSync(publicDir, { recursive: true });
        }
        // 执行复制（直接替换原有文件）
        fs.copyFileSync(DATA_FILE, PUBLIC_FILE);
        console.log(`已更新: ${PUBLIC_FILE}`);
    } catch (err) {
        console.error("同步到 public 文件夹失败:", err);
    }
}

// 发送通知的辅助函数
function sendNotify(title, message) {
    notifier.notify({
        title: title,
        message: message,
        sound: true, // 播放系统提示音
        wait: false // 不等待用户点击
    });
}

// 队列处理：防止提示冲突
async function processQueue() {
    if (isProcessing || eventQueue.length === 0) return;
    isProcessing = true;

    try {
        const { event, filePath } = eventQueue.shift();
        const relativePath = path.relative(WATCH_DIR, filePath).replace(/\\/g, '/');
        const parts = relativePath.split('/');
        const fileName = parts[parts.length - 1];
        let data = readData();

        console.log(`\n--------------------------------------`);
        console.log(`路径: ${WATCH_DIR}/${relativePath}`);

        if (event === 'add') {
            sendNotify('检测到新文件', `请在控制台确认是否添加: ${fileName}`);
            const { confirmAdd } = await inquirer.prompt([{
                type: 'confirm',
                name: 'confirmAdd',
                message: `检测到新文件 [${fileName}]，是否加入索引?`,
                default: true
            }]);

            if (confirmAdd) {
                const { desc } = await inquirer.prompt([{
                    type: 'input',
                    name: 'desc',
                    message: `请输入说明:`,
                    default: '资料存档'
                }]);
                updateTree(data, parts, desc);
                saveData(data);
                syncToPublic();
                console.log(`已更新 ./filedata.js`);
            }
        } 
        else if (event === 'unlink') {
            if (existsInTree(data, parts)) {
                sendNotify('文件已删除', `请在控制台确认移除索引: ${fileName}`);
                const { confirmDelete } = await inquirer.prompt([{
                    type: 'confirm',
                    name: 'confirmDelete',
                    message: `文件 [${fileName}] 已移除，是否同步删除索引?`,
                    default: true
                }]);

                if (confirmDelete) {
                    deleteFromTree(data, parts);
                    saveData(data);
                    syncToPublic();
                    console.log(`已清理对应索引条目`);
                }
            } else {
                console.log(`[${fileName}] 已移除，索引不存在`);
            }
        }
    } catch (err) {
        console.error("处理出错:", err);
    } finally {
        setTimeout(() => {
            isProcessing = false;
            processQueue();
        }, 200);
    }
}

// 递归更新树结构
function updateTree(tree, pathParts, desc) {
    let current = tree;
    pathParts.forEach((part, index) => {
        const isLast = index === pathParts.length - 1;
        if (isLast) {
            current[part] = {
                desc: desc,
                url: `${WATCH_DIR}/${pathParts.join('/')}`
            };
        } else {
            if (!current[part]) {
                current[part] = { 
                    type: "folder", 
                    desc: "目录", 
                    children: {} 
                };
            }
            if (!current[part].children) current[part].children = {};
            current = current[part].children;
        }
    });
}

// 递归删除树结构
function deleteFromTree(tree, pathParts) {
    let current = tree;
    for (let i = 0; i < pathParts.length; i++) {
        const part = pathParts[i];
        if (i === pathParts.length - 1) {
            if (current && current[part]) {
                delete current[part];
            }
        } else {
            current = current[part]?.children;
            if (!current) break;
        }
    }
}

// 启动监听
const watcher = chokidar.watch(WATCH_DIR, { 
    persistent: true, 
    ignoreInitial: true,
    awaitWriteFinish: { stabilityThreshold: 500 }
});

watcher.on('add', p => { 
    eventQueue.push({ event: 'add', filePath: p }); 
    processQueue(); 
});
watcher.on('unlink', p => { 
    eventQueue.push({ event: 'unlink', filePath: p }); 
    processQueue(); 
});

console.log(`同步服务已启动\n`);
sendNotify('监控服务已启动', '正在后台监控文件变化');