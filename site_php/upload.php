<?php
if (isset($_POST['submit'])) {
    $imageName = $_POST['filename'];
    if ($_POST['filename'])
        $imageName = 'kot';
    else
        $imageName = strtolower((str_replace(' ', '-', $imageName)));
    $imageTitle = $_POST['filetitle'];
    $imageDesc = $_POST['filedesc'];
    $file = $_FILES['file'];

    $fileName = $_FILES['file']['name'];
    $fileTmpName = $_FILES['file']['tmp_name'];
    $fileSize = $_FILES['file']['size'];
    $fileError = $_FILES['file']['error'];

    $fileExt = explode('.', $fileName);
    $fileActualExt = strtolower(end($fileExt));

    $allowed = array('jpg', 'jpeg', 'png');

    if (in_array($fileActualExt, $allowed)) {
        if ($fileError === 0) {
            if ($fileSize < 1000000) { 
                $fileNameNew = $imageName.".".uniqid('', true).".".$fileActualExt;
                //FIXME change here with config info
                $fileDestination = 'images/'.$fileNameNew;

                include_once "dbh.php";

                if (empty($imageTitle) || empty($imageDesc))
                {
                    header("Location: index.php?upload=empty");
                    exit();
                }
                
                move_uploaded_file($fileTmpName, $fileDestination);
                header("Location: index.php?uploadsuccess");
            } else {
                echo "File is too big to upload\n";
            }
        } else {
            echo "Error with uploading the file\n";
        }
    } else {
        echo "Wrong type of the file\n";
    }
}
?>