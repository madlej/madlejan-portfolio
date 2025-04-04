<script setup>
import { ref } from 'vue';
import { useFetch } from 'nuxt/app';

// Stav formuláře a dat
const showForm = ref(false);
const astronomerId = ref('');
const discoveryDate = ref('');
const moons = ref([]);
const error = ref(null);

// Funkce pro získání měsíců podle ID astronoma a data
const fetchMoons = async () => {
  if (!astronomerId.value || isNaN(astronomerId.value)) {
    error.value = 'Please enter a valid Astronomer ID.';
    return;
  }
  if (!discoveryDate.value) {
    error.value = 'Please select a valid Discovery Date.';
    return;
  }

  try {
    const url = `http://localhost:8080/astronomers/${astronomerId.value}/discoveryDate`;

    const response = await useFetch(url, {
      method: 'GET',
      query: { discoveryDate: discoveryDate.value },
    });

    if (response.data.value && Array.isArray(response.data.value)) {
      moons.value = response.data.value;
      error.value = null;
    } else {
      moons.value = [];
      error.value = 'No moons found.';
    }
  } catch (err) {
    console.error('Error fetching moons:', err);
    error.value = `Failed to fetch data: ${err.message}`;
  }
};
</script>

<template>
  <div class="container">
    <main class="content">
      <nav class="nav-links">
        <ul>
          <li><NuxtLink class="nav-link" to="/astronomers">Astronomers</NuxtLink></li>
          <li><NuxtLink class="nav-link" to="/moons">Moons</NuxtLink></li>
          <li><NuxtLink class="nav-link" to="/planets">Planets</NuxtLink></li>
        </ul>
      </nav>

      <button @click="showForm = !showForm" class="btn-toggle small-btn">
        {{ showForm ? 'Hide Form' : 'Find Moons by Astronomer after date' }}
      </button>

      <form v-if="showForm" @submit.prevent="fetchMoons" class="form small-form">
        <label for="astronomerId">Astronomer ID:</label>
        <input type="number" id="astronomerId" v-model="astronomerId" required />

        <label for="discoveryDate">Discovery Date:</label>
        <input type="date" id="discoveryDate" v-model="discoveryDate" required />

        <button type="submit" class="btn-submit small-submit">Search</button>
      </form>

      <div v-if="error" class="error">{{ error }}</div>

      <table v-if="moons.length" class="moons-table">
        <thead>
          <tr>
            <th>ID</th>
            <th>Name</th>
            <th>Discovery Date</th>
          </tr>
        </thead>
        <tbody>
          <tr v-for="moon in moons" :key="moon.moonId">
            <td>{{ moon.moonId }}</td>
            <td>{{ moon.nameMoon }}</td>
            <td>{{ moon.discoveryDate }}</td>
          </tr>
        </tbody>
      </table>

    </main>
  </div>
</template>

<style scoped>
/* Import font Poppins */
@import url('https://fonts.googleapis.com/css2?family=Poppins:wght@300;400;600&display=swap');

body {
  font-family: 'Poppins', sans-serif;
  margin: 0;
  padding: 0;
  background-color: #f4f7fc;
}

.container {
  display: flex;
  flex-direction: column;
  min-height: 100vh;
  padding-top: 60px;
}

.content {
  flex-grow: 1;
  padding: 20px;
  background-color: white;
  border-radius: 8px;
  box-shadow: 0 4px 12px rgba(0, 0, 0, 0.1);
}

h1, .nav-link, .form label, .form input, .error, li {
  font-family: 'Poppins', sans-serif;
}

h1 {
  font-size: 32px;
  margin-bottom: 20px;
}

.nav-links {
  margin-top: 40px;
}

.nav-links ul {
  padding: 0;
  margin: 0;
  list-style-type: none;
}

.nav-links .nav-link {
  display: block;
  color: #007bff;
  text-decoration: none;
  font-size: 18px;
  padding: 12px 20px;
  margin-bottom: 10px;
  border-radius: 4px;
  transition: background-color 0.3s, transform 0.2s;
}

.nav-links .nav-link:hover {
  background-color: #007bff;
  color: white;
  transform: translateX(5px);
}

.nav-links .nav-link:active {
  background-color: #0056b3;
  transform: translateX(0);
}

.btn-toggle {
  margin-top: 20px;
  padding: 12px 24px;
  font-size: 16px;
  cursor: pointer;
  background-color: #007bff;
  color: white;
  border: none;
  border-radius: 8px;
  transition: background-color 0.3s ease, transform 0.2s;
}

.btn-toggle:hover {
  background-color: #0056b3;
  transform: translateY(-2px);
}

.btn-toggle.small-btn {
  font-size: 14px;
  padding: 8px 16px;
  margin-top: 40px;
  align-self: flex-end;
}

.form {
  margin-top: 20px;
  display: flex;
  flex-direction: column;
  gap: 15px;
  max-width: 500px;
}

.form.small-form label {
  font-size: 14px;
}

.form.small-form input {
  padding: 8px;
  font-size: 14px;
  border: 1px solid #ddd;
  border-radius: 8px;
  outline: none;
  transition: border-color 0.3s ease;
}

.form.small-form input:focus {
  border-color: #007bff;
}

.btn-submit {
  padding: 12px 24px;
  font-size: 18px;
  background-color: #007bff;
  color: white;
  border: none;
  border-radius: 8px;
  cursor: pointer;
}

.btn-submit.small-submit {
  padding: 8px 16px;
  font-size: 14px;
}

.btn-submit:hover {
  background-color: #0056b3;
}

.error {
  margin-top: 10px;
  color: red;
  font-weight: bold;
}

/* Updated table styling with consistent font */
.moons-table {
  width: 100%;
  border-collapse: collapse;
  margin-top: 20px;
  background-color: #f9f9f9;
  font-family: 'Poppins', sans-serif; /* Ensure consistent font */
}

.moons-table th {
  background-color: #007bff;
  color: white;
  text-align: left;
  padding: 12px;
  font-size: 14px;
  border-bottom: 2px solid #ddd;
}

.moons-table td {
  padding: 12px;
  font-size: 14px;
  color: #333;
  border-bottom: 1px solid #ddd;
}

.moons-table tr:nth-child(even) {
  background-color: #f4f7fc;
}

.moons-table tr:hover {
  background-color: #eef3fc;
  cursor: pointer;
}

ul {
  list-style: none;
  padding: 0;
}

li {
  font-size: 16px;
  color: #333;
  margin: 8px 0;
}

@media (max-width: 768px) {
  .container {
    padding-top: 20px;
  }

  .content {
    margin-left: 0;
    padding: 20px;
  }

  .nav-links .nav-link {
    font-size: 16px;
  }

  .form {
    max-width: 100%;
  }
}
</style>
