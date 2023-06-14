<?php
     // doc user input
    include("config.php");
    $sql = "UPDATE datastudent SET timein = null WHERE 1";
    mysqli_query($conn, $sql);
    mysqli_close($conn);
?>