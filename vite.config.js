import { defineConfig } from 'vite'
import vue from '@vitejs/plugin-vue'

export default defineConfig(({ command }) => {
  return {
    plugins: [vue()],
    base: command === 'serve' ? '/' : './',
    
    build: {
      rollupOptions: {
        output: {
          assetFileNames: (assetInfo) => {
            // 匹配图片后缀
            if (/\.(png|jpe?g|gif|svg|webp|ico)$/i.test(assetInfo.name)) {
              // 这里的 [name] 会取原始文件名
              return 'assets/[name]-[hash][extname]';
            }
            // 其余维持原样
            return 'assets/[name]-[hash][extname]';
          },
        },
      },
    },
  }
})