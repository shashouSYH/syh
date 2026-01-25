import { defineConfig } from 'vite'
import vue from '@vitejs/plugin-vue'

export default defineConfig(({ command }) => {
  return {
    plugins: [vue()],
    // 判断逻辑：
    // 如果是开发模式 (serve)，路径用 '/'
    // 如果是打包模式 (build)，路径用 './'
    base: command === 'serve' ? '/' : './',
  }
})