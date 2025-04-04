<script setup>
import { ref, onMounted } from 'vue';

const items = ref([]);
const newPlanet = ref({
  namePlanet: '',
  typePlanet: '',
  mass: '',
});
const error = ref(null);
const successMessage = ref(null);
const showForm = ref(false); // Form toggle

onMounted(async () => {
  await fetchPlanets();
});

const fetchPlanets = async () => {
  try {
    const response = await $fetch('http://localhost:8080/planets');
    if (response && Array.isArray(response)) {
      items.value = response;
    } else {
      console.error('No planets found or data is not an array');
    }
  } catch (err) {
    console.error('Error fetching items:', err);
    error.value = 'Failed to fetch planets. Please try again later.';
  }
};

const addPlanet = async () => {
  successMessage.value = null;
  error.value = null;

  try {
    const response = await $fetch('http://localhost:8080/planets', {
      method: 'POST',
      body: {
        namePlanet: newPlanet.value.namePlanet,
        typePlanet: newPlanet.value.typePlanet || 'Unknown',
        mass: newPlanet.value.mass || null,
      },
    });

    if (response) {
      successMessage.value = `Planet ${response.namePlanet} successfully added!`;
      newPlanet.value = { namePlanet: '', typePlanet: '', mass: '' }; // Reset form
      await fetchPlanets(); // Refresh the list
      showForm.value = false; // Hide form
    }
  } catch (err) {
    console.error('Error adding planet:', err);
    error.value = 'Failed to add the planet. Please try again.';
  }
};

const deletePlanet = async (planetId) => {
  const confirmDeletion = confirm(`Are you sure you want to delete the planet "${planetId}"?`);
  if (!confirmDeletion) return;

  try {
    await $fetch(`http://localhost:8080/planets/${planetId}`, {
      method: 'DELETE',
    });
    successMessage.value = `Planet ${planetId} successfully deleted!`;
    await fetchPlanets(); // Refresh the list
  } catch (err) {
    console.error(`Error deleting planet with id ${planetId}:`, err);
    error.value = `Failed to delete planet "${planetId}". Please try again.`;
  }
};
</script>

<template>
  <div class="container">
    <h1>Planets</h1>

    <!-- Success or Error Messages -->
    <p v-if="successMessage" class="success-message">{{ successMessage }}</p>
    <p v-if="error" class="error-message">{{ error }}</p>

    <!-- Toggle Button for Add Planet -->
    <button @click="showForm = !showForm" class="toggle-button">
      {{ showForm ? 'Hide Add Planet' : 'Add Planet' }}
    </button>

    <!-- Form to Add a New Planet -->
    <div v-if="showForm" class="form-container">
      <h2>Add New Planet</h2>
      <form @submit.prevent="addPlanet">
        <div class="form-group">
          <label for="namePlanet">Name:</label>
          <input
            id="namePlanet"
            v-model="newPlanet.namePlanet"
            placeholder="Planet Name"
            required
          />
        </div>
        <div class="form-group">
          <label for="typePlanet">Type:</label>
          <input
            id="typePlanet"
            v-model="newPlanet.typePlanet"
            placeholder="Type (e.g., Terrestrial, Gas Giant)"
          />
        </div>
        <div class="form-group">
          <label for="mass">Mass (kg):</label>
          <input
            id="mass"
            v-model="newPlanet.mass"
            placeholder="Mass (e.g., 5.972e24)"
          />
        </div>
        <button type="submit" class="submit-button">Add Planet</button>
      </form>
    </div>

    <!-- Loading or Table -->
    <p v-if="items.length === 0 && !error">Loading...</p>

    <table v-if="items.length > 0" class="planet-table">
      <thead>
        <tr>
          <th>Name</th>
          <th>Type</th>
          <th>Mass</th>
          <th>Moons</th>
          <th>Actions</th>
        </tr>
      </thead>
      <tbody>
        <tr v-for="item in items" :key="item.namePlanet">
          <td>{{ item.namePlanet }}</td>
          <td>{{ item.typePlanet || 'Unknown' }}</td>
          <td>{{ item.mass ? `${item.mass} kg` : 'Unknown' }}</td>
          <td>{{ item.moonsIds ? item.moonsIds.join(', ') : 'No moons' }}</td>
          <td>
            <button
              class="delete-button"
              @click="deletePlanet(item.namePlanet)"
              title="Delete this planet"
            >
              🗑️
            </button>
          </td>
        </tr>
      </tbody>
    </table>
  </div>
</template>

<style scoped>
/* Stejný styl jako dříve */
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

.toggle-button {
  background-color: #007bff;
  color: white;
  padding: 10px 20px;
  border: none;
  border-radius: 4px;
  font-size: 14px;
  cursor: pointer;
  transition: background-color 0.3s;
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

.planet-table {
  width: 100%;
  border-collapse: collapse;
  margin-top: 20px;
}

.planet-table th,
.planet-table td {
  padding: 12px;
  border-bottom: 1px solid #ddd;
  text-align: left;
}

.planet-table th {
  background-color: #007bff;
  color: white;
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
