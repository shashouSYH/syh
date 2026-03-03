<template>
  <aside class="sidebar">
    <img
      src="/avatar.png"
      class="avatar"
      onerror="this.src='https://img.icons8.com/fluency/96/user-male-circle.png'"
    >

    <h1 style="margin:10px 0 5px; font-size:1.5rem;">殺手</h1>

    <p style="font-size:0.9rem;color:#555;line-height:1.6;">
      君子坐而论道<br>
      少年起而行之
    </p>

    <ul class="nav-menu">
      <li>
        <a 
          href="#intro" 
          class="nav-link" 
          :class="{ active: activeSection === 'intro' }"
          @click="setActive('intro')"
        >网站介绍</a>
      </li>
      <li>
        <a 
          href="#files" 
          class="nav-link" 
          :class="{ active: activeSection === 'files' }"
          @click="setActive('files')"
        >学习资料</a>
      </li>
    </ul>
  </aside>
</template>

<script setup>
import { ref, onMounted, onUnmounted } from 'vue';

// 默认选中的 section
const activeSection = ref('intro');

// 手动点击切换（可选，滚动监听会自动处理）
const setActive = (id) => {
  activeSection.value = id;
};

// 滚动监听逻辑
const handleScroll = () => {
  const sections = ['intro', 'files'];
  const scrollPosition = window.scrollY + 100; // 偏移量，防止卡在边缘

  for (const id of sections) {
    const element = document.getElementById(id);
    if (element) {
      const top = element.offsetTop;
      const height = element.offsetHeight;

      if (scrollPosition >= top && scrollPosition < top + height) {
        activeSection.value = id;
      }
    }
  }
};

onMounted(() => {
  window.addEventListener('scroll', handleScroll);
});

onUnmounted(() => {
  window.removeEventListener('scroll', handleScroll);
});
</script>

<style scoped>
/* 确保你的 CSS 中有 .active 的样式 */
.nav-link.active {
  background: rgba(0, 123, 255, 0.1);
  color: #007bff; /* 或者你的 --primary-color */
  font-weight: bold;
}
</style>