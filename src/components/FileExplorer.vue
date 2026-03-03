<template>
  <section id="files" class="section-card">
    <h2 class="section-title">学习资料</h2>

    <p style="font-size:0.85rem;color:#888;margin-bottom:10px;">
      双击文件夹进入, 双击文件下载, pdf默认在浏览器中打开
    </p>

    <div class="w11-explorer">
      <div class="w11-toolbar">
        <button class="w11-back-btn" @click="goBack">
          <img src="https://img.icons8.com/material-outlined/48/left.png"
               style="width:20px;height:20px;">
        </button>

        <div class="w11-addr" v-if="!searchQuery">
          <span class="crumb" @click="jumpTo(-1)">首页</span>
          <template v-for="(pathName, idx) in currentPath" :key="idx">
            <span style="color:#ccc"> > </span>
            <span class="crumb" @click="jumpTo(idx)">
              {{ pathName }}
            </span>
          </template>
        </div>
        <div class="w11-addr" v-else>
          <span class="crumb" style="color: #666">搜索结果: "{{ searchQuery }}"</span>
        </div>
        
        <div class="w11-search" style="margin-left: auto; display: flex;">
          <input type="text" v-model="searchQuery" placeholder="搜索文件或文件夹..." class="w11-search-input" />
        </div>
      </div>

      <div class="w11-content">
        <div class="w11-header">
          <div>名称</div>
          <div>说明</div>
        </div>

        <div v-for="(info, name) in currentData"
             :key="info.virtualPath || name"
             class="w11-item"
             :class="{ selected: selectedName === (info.virtualPath || name) }"
             @click="selectedName = (info.virtualPath || name)"
             @dblclick="handleDblClick(name, info)">
          <div class="w11-item-name">
            <img :src="getIcon(name, info)" style="width:20px;height:20px;">
            <span :title="info.virtualPath ? info.virtualPath.join(' > ') + ' > ' + name : name">{{ name }}</span>
          </div>
          <div class="w11-item-desc" :title="info.desc || '—'">
            {{ info.desc || '—' }}
          </div>
        </div>
      </div>
    </div>
  </section>
</template>

<script setup>
import { ref, computed } from 'vue'

// 在 Vite 开发模式下，FILE_DB 挂在 window 上需要安全访问
const db = window.FILE_DB || {}

const currentPath = ref([])
const selectedName = ref('')
const searchQuery = ref('')

const iconMap = {
  folder: "https://img.icons8.com/fluency/48/folder-invoices.png",
  pdf: "https://img.icons8.com/?size=100&id=l0vjMqIboTRs&format=png&color=000000",
  ppt: "https://img.icons8.com/fluency/48/microsoft-powerpoint-2019.png",
  pptx: "https://img.icons8.com/fluency/48/microsoft-powerpoint-2019.png",
  xls: "https://img.icons8.com/fluency/48/microsoft-excel-2019.png",
  xlsx: "https://img.icons8.com/fluency/48/microsoft-excel-2019.png",
  doc: "https://img.icons8.com/fluency/48/microsoft-word-2019.png",
  docx: "https://img.icons8.com/fluency/48/microsoft-word-2019.png",
  zip: "https://img.icons8.com/?size=100&id=hwJDMFCJ6XVn&format=png&color=000000",
  mp3: "https://img.icons8.com/?size=100&id=zAzROOhkw_7N&format=png&color=000000",
  mkv: "https://img.icons8.com/?size=100&id=114331&format=png&color=000000",
  mp4: "https://img.icons8.com/?size=100&id=114331&format=png&color=000000",
  default: "https://img.icons8.com/fluency/48/file.png"
}

function searchFiles(node, query, pathAcc, result) {
  for (const [name, info] of Object.entries(node)) {
    const isMatch = name.toLowerCase().includes(query) || (info.desc && info.desc.toLowerCase().includes(query));
    if (isMatch) {
      result[name + (info.virtualPath ? Math.random().toString().slice(2, 6) : "")] = {
        ...info,
        virtualPath: pathAcc
      };
    }
    if (info.children) {
      searchFiles(info.children, query, [...pathAcc, name], result);
    }
  }
}

const currentData = computed(() => {
  const query = searchQuery.value.trim().toLowerCase();
  if (query) {
    const searchResults = {};
    searchFiles(db, query, [], searchResults);
    return searchResults;
  }
  
  let data = db
  currentPath.value.forEach(p => {
    if (data[p] && data[p].children) {
      data = data[p].children
    }
  })
  return data
})

function handleDblClick(name, info) {
  if (info.type === 'folder') {
    if (info.virtualPath) {
      // 如果是搜索结果中的文件夹，跳转到该文件夹的实际路径
      currentPath.value = [...info.virtualPath, name]
      searchQuery.value = ''
    } else {
      currentPath.value.push(name)
    }
    selectedName.value = ''
  } else if (info.url) {
    window.open(info.url, '_blank')
  }
}

function goBack() {
  if (currentPath.value.length > 0) {
    currentPath.value.pop()
    selectedName.value = ''
  }
}

function jumpTo(index) {
  if (index === -1) {
    currentPath.value = []
  } else {
    currentPath.value = currentPath.value.slice(0, index + 1)
  }
  selectedName.value = ''
}

function getIcon(name, info) {
  if (info.type === 'folder') return iconMap.folder
  const ext = name.split('.').pop().toLowerCase()
  return iconMap[ext] || iconMap.default
}
</script>