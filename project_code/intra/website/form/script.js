// Function to add a name to the list
function addName() {
    const nameInput = document.getElementById('nameInput');
    const name = nameInput.value.trim();

    if (name !== '') {
        const nameList = document.getElementById('nameList');
        const listItem = document.createElement('li');
        listItem.textContent = name;
        nameList.appendChild(listItem);
        nameInput.value = '';

        // Save the names to local storage
        saveNamesToLocalStorage(name);
    }
}

// Function to save the names to local storage
function saveNamesToLocalStorage(name) {
    const names = getNamesFromLocalStorage();
    names.push(name);
    localStorage.setItem('names', JSON.stringify(names));
}

// Function to get the names from local storage
function getNamesFromLocalStorage() {
    const namesJSON = localStorage.getItem('names');
    return namesJSON ? JSON.parse(namesJSON) : [];
}

// Populate the list with names from local storage on page load
document.addEventListener('DOMContentLoaded', () => {
    const names = getNamesFromLocalStorage();
    const nameList = document.getElementById('nameList');

    names.forEach(name => {
        const listItem = document.createElement('li');
        listItem.textContent = name;
        nameList.appendChild(listItem);
    });
});
