<script setup>
import { ref, onMounted } from 'vue';

const items = ref([]);
const newAstronomer = ref({
  realName: '',
  email: '',
});
const editingAstronomer = ref(null);
const error = ref(null);
const successMessage = ref(null);
const showForm = ref(false);

const searchAstronomerId = ref(''); // Pro vyhledávací lištu
const searchResult = ref(null); // Výsledek hledání

onMounted(async () => {
  await fetchAstronomers();
});

const fetchAstronomers = async () => {
  try {
    const response = await $fetch('http://localhost:8080/astronomers');
    if (response && Array.isArray(response)) {
      items.value = response;
    } else {
      console.error('No astronomers found or data is not an array');
    }
  } catch (err) {
    console.error('Error fetching items:', err);
    error.value = 'Failed to fetch astronomers. Please try again later.';
  }
};

const searchAstronomer = async () => {
  if (!searchAstronomerId.value) {
    searchResult.value = null;
    error.value = 'Please enter a valid Astronomer ID.';
    return;
  }

  try {
    const response = await $fetch(`http://localhost:8080/astronomers/${searchAstronomerId.value}`);
    if (response) {
      searchResult.value = response;
      error.value = null;
    } else {
      searchResult.value = null;
      error.value = `Astronomer with ID ${searchAstronomerId.value} not found.`;
    }
  } catch (err) {
    console.error(`Error fetching astronomer with ID ${searchAstronomerId.value}:`, err);
    error.value = `Failed to fetch astronomer with ID ${searchAstronomerId.value}. Please try again.`;
    searchResult.value = null;
  }
};

const addAstronomer = async () => {
  successMessage.value = null;
  error.value = null;

  try {
    const response = await $fetch('http://localhost:8080/astronomers', {
      method: 'POST',
      headers: {
        'Content-Type': 'application/json',
      },
      body: JSON.stringify({
        astronomerId: 0,
        realName: newAstronomer.value.realName,
        email: newAstronomer.value.email,
        moonsIds: [],
      }),
    });

    if (response) {
      successMessage.value = `Astronomer ${response.realName} successfully added!`;
      newAstronomer.value = { realName: '', email: '' };
      await fetchAstronomers();
      showForm.value = false;
    }
  } catch (err) {
    console.error('Error adding astronomer:', err);
    error.value = 'Failed to add the astronomer. Please try again.';
  }
};

const deleteAstronomer = async (astronomerId) => {
  const confirmDeletion = confirm(`Are you sure you want to delete the astronomer with ID "${astronomerId}"?`);
  if (!confirmDeletion) return;

  try {
    await $fetch(`http://localhost:8080/astronomers/${astronomerId}`, {
      method: 'DELETE',
    });
    successMessage.value = `Astronomer with ID ${astronomerId} successfully deleted!`;
    await fetchAstronomers();
  } catch (err) {
    console.error(`Error deleting astronomer with ID ${astronomerId}:`, err);
    error.value = `Failed to delete astronomer with ID "${astronomerId}". Please try again.`;
  }
};

const startEditAstronomer = (astronomer) => {
  editingAstronomer.value = astronomer.astronomerId;
  newAstronomer.value = {
    realName: astronomer.realName,
    email: astronomer.email,
  };
  showForm.value = true;
};

const updateAstronomer = async () => {
  if (!editingAstronomer.value) return;

  try {
    const response = await $fetch(`http://localhost:8080/astronomers/${editingAstronomer.value}`, {
      method: 'PUT',
      headers: {
        'Content-Type': 'application/json',
      },
      body: JSON.stringify({
        astronomerId: editingAstronomer.value,
        realName: newAstronomer.value.realName,
        email: newAstronomer.value.email,
        moonsIds: newAstronomer.value.moonsIds
          ? newAstronomer.value.moonsIds.split(',').map((id) => parseInt(id.trim()))
          : [],
      }),
    });

    if (response) {
      successMessage.value = `Astronomer ${response.realName} successfully updated!`;
      editingAstronomer.value = null;
      newAstronomer.value = { realName: '', email: '', moonsIds: [] };
      await fetchAstronomers();
      showForm.value = false;
    }
  } catch (err) {
    console.error('Error updating astronomer:', err);
    error.value = 'Failed to update the astronomer. Please try again.';
  }
};
</script>

<template>
  <div class="container">
    <h1>Astronomers</h1>

    <!-- Vyhledávací lišta -->
    <div class="search-bar">
      <input
        type="number"
        v-model="searchAstronomerId"
        placeholder="Enter Astronomer ID"
      />
      <button @click="searchAstronomer" class="search-button">Search</button>
    </div>

    <!-- Výsledek hledání -->
    <div v-if="searchResult" class="search-result">
      <h2>Search Result:</h2>
      <p><strong>ID:</strong> {{ searchResult.astronomerId }}</p>
      <p><strong>Name:</strong> {{ searchResult.realName }}</p>
      <p><strong>Email:</strong> {{ searchResult.email }}</p>
      <p><strong>Moons:</strong> {{ searchResult.moonsIds ? searchResult.moonsIds.join(', ') : 'No Moons' }}</p>
    </div>

    <!-- Success or Error Messages -->
    <p v-if="successMessage" class="success-message">{{ successMessage }}</p>
    <p v-if="error" class="error-message">{{ error }}</p>

    <!-- Toggle Button for Add or Edit Astronomer -->
    <button @click="showForm = !showForm" class="toggle-button">
      {{ showForm ? (editingAstronomer ? 'Hide Edit Astronomer' : 'Hide Add Astronomer') : 'Add Astronomer' }}
    </button>

    <!-- Form to Add or Edit an Astronomer -->
    <div v-if="showForm" class="form-container">
      <h2>{{ editingAstronomer ? 'Edit Astronomer' : 'Add New Astronomer' }}</h2>
      <form @submit.prevent="editingAstronomer ? updateAstronomer() : addAstronomer()">
        <div class="form-group">
          <label for="realName">Name:</label>
          <input id="realName" v-model="newAstronomer.realName" placeholder="Full Name" required />
        </div>
        <div class="form-group">
          <label for="email">Email:</label>
          <input id="email" v-model="newAstronomer.email" placeholder="Email" required />
        </div>
        <button type="submit" class="submit-button">
          {{ editingAstronomer ? 'Update Astronomer' : 'Add Astronomer' }}
        </button>
      </form>
    </div>

    <!-- Loading or Table -->
    <p v-if="items.length === 0 && !error">Loading...</p>

    <table v-if="items.length > 0" class="astronomer-table">
      <thead>
        <tr>
          <th>ID</th>
          <th>Name</th>
          <th>Email</th>
          <th>Moons</th>
          <th>Actions</th>
        </tr>
      </thead>
      <tbody>
        <tr v-for="item in items" :key="item.astronomerId">
          <td>{{ item.astronomerId }}</td>
          <td>{{ item.realName }}</td>
          <td>{{ item.email }}</td>
          <td>{{ item.moonsIds ? item.moonsIds.join(', ') : 'No Moons' }}</td>
          <td>
            <button class="edit-button" @click="startEditAstronomer(item)" title="Edit this astronomer">✏️</button>
            <button class="delete-button" @click="deleteAstronomer(item.astronomerId)" title="Delete this astronomer">🗑️</button>
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

/* Vyhledávací lišta */
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

/* Toggle Button */
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

.astronomer-table {
  width: 100%;
  border-collapse: collapse;
  margin-top: 20px;
}

.astronomer-table th,
.astronomer-table td {
  padding: 12px;
  border-bottom: 1px solid #ddd;
  text-align: left;
}

.astronomer-table th {
  background-color: #007bff;
  color: white;
}

.edit-button {
  background: none;
  border: none;
  color: green;
  font-size: 18px;
  cursor: pointer;
}

.edit-button:hover {
  color: darkgreen;
}

.delete-button {
  background: none;
  border: none;
  color: red;
  font-size: 18px;
  cursor: pointer;
}

.delete-button:hover {
  color: darkred;
}
</style>
