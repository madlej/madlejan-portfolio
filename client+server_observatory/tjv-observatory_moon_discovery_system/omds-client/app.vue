<!-- app.vue -->
<template>
  <div>
    <header>
      <a href="/" class="home-link">
        <i class="fas fa-home"></i> 
      </a>
      <h1>Observatory Moon Discovery System</h1>    
    </header>

    <main>
      <NuxtPage />
    </main>

  </div>
</template>

<script setup>
</script>

<style scoped>

body {
  font-family: 'Poppins', sans-serif;
}

h1 {
  font-family: 'Poppins', sans-serif;
  font-size: 36px;  /* Adjust size as needed */
  font-weight: 600; /* Bold weight to make the heading pop */
  color: #333;      /* Dark color for good contrast */
  margin-bottom: 20px; /* Space below the heading */
}

/* Style the header */
header {
  display: flex;
  align-items: center;
  justify-content: space-between;
  padding: 10px;
  background-color: #f8f8f8;
}

h1 {
  font-size: 24px;
  margin: 0;
}

/* Style the home link */
.home-link {
  font-size: 30px;  /* Adjust size of the icon */
  color: #333;
  text-decoration: none;
  margin-right: 15px;
  transition: color 0.3s ease;
}

/* Change the icon color on hover */
.home-link:hover {
  color: #2889e3;
}
</style>