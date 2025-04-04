<script setup>
import { ref, onMounted } from 'vue';

const items = ref([]);
const newMoon = ref({
  moonId: 0,
  discoveryDate: '',
  nameMoon: '',
  diameter: 0,
  astronomersIds: [],
  planetId: '',
});
const editingMoon = ref(null);
const error = ref(null);
const successMessage = ref(null);
const showForm = ref(false);

const searchMoonId = ref(''); // Pro vyhledávací lištu
const searchResult = ref(null); // Výsledek hledání

onMounted(async () => {
  await fetchMoons();
});

const fetchMoons = async () => {
  try {
    const response = await $fetch('http://localhost:8080/moons');
    if (response && Array.isArray(response)) {
      items.value = response;
    } else {
      console.error('No moons found or data is not an array');
    }
  } catch (err) {
    console.error('Error fetching items:', err);
    error.value = 'Failed to fetch moons. Please try again later.';
  }
};

const searchMoon = async () => {
  if (!searchMoonId.value) {
    searchResult.value = null;
    error.value = 'Please enter a valid Moon ID.';
    return;
  }

  try {
    const response = await $fetch(`http://localhost:8080/moons/${searchMoonId.value}`);
    if (response) {
      searchResult.value = response;
      error.value = null;
    } else {
      searchResult.value = null;
      error.value = `Moon with ID ${searchMoonId.value} not found.`;
    }
  } catch (err) {
    console.error(`Error fetching moon with ID ${searchMoonId.value}:`, err);
    error.value = `Failed to fetch moon with ID ${searchMoonId.value}. Please try again.`;
    searchResult.value = null;
  }
};

const addMoon = async () => {
  successMessage.value = null;
  error.value = null;

  try {
    const response = await $fetch('http://localhost:8080/moons', {
      method: 'POST',
      headers: {
        'Content-Type': 'application/json',
      },
      body: JSON.stringify({
        moonId: 0,
        discoveryDate: newMoon.value.discoveryDate || null,
        nameMoon: newMoon.value.nameMoon,
        diameter: newMoon.value.diameter || null,
        astronomersIds: newMoon.value.astronomersIds
          ? newMoon.value.astronomersIds.split(',').map((id) => parseInt(id.trim()))
          : [],
        planetId: newMoon.value.planetId || null,
      }),
    });

    if (response) {
      successMessage.value = `Moon ${response.nameMoon} successfully added!`;
      resetForm();
      await fetchMoons();
      showForm.value = false;
    }
  } catch (err) {
    console.error('Error adding moon:', err);
    error.value = 'Failed to add the moon. Please try again.';
  }
};

const deleteMoon = async (moonId) => {
  const confirmDeletion = confirm(`Are you sure you want to delete the moon with ID "${moonId}"?`);
  if (!confirmDeletion) return;

  try {
    await $fetch(`http://localhost:8080/moons/${moonId}`, {
      method: 'DELETE',
    });
    successMessage.value = `Moon with ID ${moonId} successfully deleted!`;
    await fetchMoons();
  } catch (err) {
    console.error(`Error deleting moon with ID ${moonId}:`, err);
    error.value = `Failed to delete moon with ID "${moonId}". Please try again.`;
  }
};

const startEditMoon = (moon) => {
  editingMoon.value = moon.moonId;
  newMoon.value = {
    moonId: moon.moonId,
    discoveryDate: moon.discoveryDate,
    nameMoon: moon.nameMoon,
    diameter: moon.diameter,
    astronomersIds: moon.astronomersIds ? moon.astronomersIds.join(', ') : '',
    planetId: moon.planetId,
  };
  showForm.value = true;
};

const updateMoon = async () => {
  if (!editingMoon.value) return;

  try {
    const response = await $fetch(`http://localhost:8080/moons/${editingMoon.value}`, {
      method: 'PUT',
      headers: {
        'Content-Type': 'application/json',
      },
      body: JSON.stringify({
        moonId: editingMoon.value,
        discoveryDate: newMoon.value.discoveryDate || null,
        nameMoon: newMoon.value.nameMoon,
        diameter: newMoon.value.diameter || null,
        astronomersIds: newMoon.value.astronomersIds
          ? newMoon.value.astronomersIds.split(',').map((id) => parseInt(id.trim()))
          : [],
        planetId: newMoon.value.planetId || null,
      }),
    });

    if (response) {
      successMessage.value = `Moon ${response.nameMoon} successfully updated!`;
      resetForm();
      await fetchMoons();
      showForm.value = false;
    }
  } catch (err) {
    console.error('Error updating moon:', err);
    error.value = 'Failed to update the moon. Please try again.';
  }
};

const toggleForm = () => {
  if (showForm.value && !editingMoon.value) {
    resetForm();
  }
  showForm.value = !showForm.value;
};

const resetForm = () => {
  editingMoon.value = null;
  newMoon.value = {
    moonId: 0,
    discoveryDate: '',
    nameMoon: '',
    diameter: 0,
    astronomersIds: [],
    planetId: '',
  };
};

const formatDate = (date) => (date ? date : 'N/A');
const formatDiameter = (diameter) => (diameter ? `${diameter} km` : 'Unknown');
</script>

<template>
  <div class="container">
    <h1>Moons</h1>

    <!-- Vyhledávací lišta -->
    <div class="search-bar">
      <input
        type="number"
        v-model="searchMoonId"
        placeholder="Enter Moon ID"
      />
      <button @click="searchMoon" class="search-button">Search</button>
    </div>

    <!-- Výsledek hledání -->
    <div v-if="searchResult" class="search-result">
      <h2>Search Result:</h2>
      <p><strong>ID:</strong> {{ searchResult.moonId }}</p>
      <p><strong>Name:</strong> {{ searchResult.nameMoon }}</p>
      <p><strong>Discovery Date:</strong> {{ searchResult.discoveryDate || 'Unknown' }}</p>
      <p><strong>Diameter:</strong> {{ searchResult.diameter ? searchResult.diameter + ' km' : 'Unknown' }}</p>
      <p><strong>Astronomers:</strong> {{ searchResult.astronomersIds ? searchResult.astronomersIds.join(', ') : 'No astronomers' }}</p>
      <p><strong>Planet:</strong> {{ searchResult.planetId || 'Unknown' }}</p>
    </div>

    <!-- Success or Error Messages -->
    <p v-if="successMessage" class="success-message">{{ successMessage }}</p>
    <p v-if="error" class="error-message">{{ error }}</p>

    <!-- Toggle Button for Add or Edit Moon -->
    <button @click="toggleForm" class="toggle-button">
      {{ showForm ? (editingMoon ? 'Hide Edit Moon' : 'Hide Add Moon') : 'Add Moon' }}
    </button>

    <!-- Form for Add or Edit Moon -->
    <div v-if="showForm" class="form-container">
      <h2>{{ editingMoon ? 'Edit Moon' : 'Add New Moon' }}</h2>
      <form @submit.prevent="editingMoon ? updateMoon() : addMoon()">
        <div class="form-group">
          <label for="nameMoon">Name:</label>
          <input id="nameMoon" v-model="newMoon.nameMoon" placeholder="Moon Name" required />
        </div>
        <div class="form-group">
          <label for="discoveryDate">Discovery Date:</label>
          <input id="discoveryDate" type="date" v-model="newMoon.discoveryDate" />
        </div>
        <div class="form-group">
          <label for="diameter">Diameter (km):</label>
          <input id="diameter" type="number" v-model="newMoon.diameter" placeholder="Diameter" />
        </div>
        <div class="form-group">
          <label for="astronomersIds">Astronomers (comma-separated IDs):</label>
          <input id="astronomersIds" v-model="newMoon.astronomersIds" placeholder="E.g., 1, 2, 3" />
        </div>
        <div class="form-group">
          <label for="planetId">Planet ID:</label>
          <input id="planetId" v-model="newMoon.planetId" placeholder="E.g., Venus" />
        </div>
        <button type="submit" class="submit-button">
          {{ editingMoon ? 'Update Moon' : 'Add Moon' }}
        </button>
      </form>
    </div>

    <!-- Loading or Table -->
    <p v-if="items.length === 0 && !error">Loading...</p>

    <table v-if="items.length > 0" class="moon-table">
      <thead>
        <tr>
          <th>ID</th>
          <th>Name</th>
          <th>Discovery Date</th>
          <th>Diameter</th>
          <th>Astronomers</th>
          <th>Planet</th>
          <th>Actions</th>
        </tr>
      </thead>
      <tbody>
        <tr v-for="item in items" :key="item.moonId">
          <td>{{ item.moonId }}</td>
          <td>{{ item.nameMoon }}</td>
          <td>{{ formatDate(item.discoveryDate) }}</td>
          <td>{{ formatDiameter(item.diameter) }}</td>
          <td>{{ item.astronomersIds ? item.astronomersIds.join(', ') : 'No astronomers' }}</td>
          <td>{{ item.planetId || 'Unknown' }}</td>
          <td>
            <button class="edit-button" @click="startEditMoon(item)" title="Edit this moon">✏️</button>
            <button class="delete-button" @click="deleteMoon(item.moonId)" title="Delete this moon">🗑️</button>
          </td>
        </tr>
      </tbody>
    </table>
  </div>
</template>


<style scoped>
.container {
  font-family: 'Poppins', sans-serif;
  margin: 20px;
}

h1 {
  font-size: 32px;
  margin-bottom: 20px;
}

.success-message {
  color: green;
  margin-bottom: 10px;
}

.error-message {
  color: red;
  margin-bottom: 10px;
}

.search-bar {
  display: flex;
  align-items: center;
  margin-bottom: 20px;
}

.search-bar input {
  padding: 10px;
  font-size: 14px;
  border: 1px solid #ccc;
  border-radius: 4px;
  margin-right: 10px;
  flex: 1;
}

.search-bar .search-button {
  background-color: #007bff;
  color: white;
  padding: 10px 20px;
  border: none;
  border-radius: 4px;
  font-size: 14px;
  cursor: pointer;
  transition: background-color 0.3s ease-in-out;
}

.search-bar .search-button:hover {
  background-color: #0056b3;
}

.search-result {
  margin-bottom: 20px;
  padding: 15px;
  background-color: #f9f9f9;
  border: 1px solid #ddd;
  border-radius: 4px;
}

.search-result h2 {
  font-size: 20px;
  margin-bottom: 10px;
}

.toggle-button {
  background-color: #007bff;
  color: white;
  padding: 10px 20px;
  border: none;
  border-radius: 4px;
  font-size: 14px;
  cursor: pointer;
  transition: background-color 0.3s ease-in-out;
  margin-bottom: 20px;
}

.toggle-button:hover {
  background-color: #0056b3;
}

.form-container {
  margin-top: 20px;
  padding: 20px;
  background-color: #f9f9f9;
  border-radius: 8px;
  box-shadow: 0 4px 12px rgba(0, 0, 0, 0.1);
}

h2 {
  font-size: 24px;
  margin-bottom: 15px;
}

.form-group {
  margin-bottom: 15px;
}

label {
  display: block;
  font-size: 14px;
  margin-bottom: 5px;
}

input {
  width: 100%;
  padding: 10px;
  font-size: 14px;
  border: 1px solid #ccc;
  border-radius: 4px;
  box-sizing: border-box;
  transition: border-color 0.3s;
}

input:focus {
  border-color: #007bff;
  outline: none;
}

.submit-button {
  background-color: #007bff;
  color: white;
  padding: 10px 20px;
  border: none;
  border-radius: 4px;
  font-size: 14px;
  cursor: pointer;
  transition: background-color 0.3s;
}

.submit-button:hover {
  background-color: #0056b3;
}

.moon-table {
  width: 100%;
  border-collapse: collapse;
  margin-top: 20px;
}

.moon-table th,
.moon-table td {
  padding: 12px;
  border-bottom: 1px solid #ddd;
  text-align: left;
}

.moon-table th {
  background-color: #007bff;
  color: white;
}

.edit-button {
  background: none;
  border: none;
  color: #ff5733;
  font-size: 18px;
  cursor: pointer;
  padding: 0;
}

.edit-button:hover {
  color: #c4421a;
}

.delete-button {
  background: none;
  border: none;
  color: red;
  font-size: 18px;
  cursor: pointer;
  padding: 0;
}

.delete-button:hover {
  color: darkred;
}
</style>
