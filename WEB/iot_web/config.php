<?php
// Connect to database
$server = "localhost";
$user = "Sang"; 
$pass = "123";
$dbname = "attendanceSystem";

$conn = mysqli_connect($server,$user,$pass,$dbname);

// Check connection
if($conn === false){    
    die("ERROR: Could not connect. " . mysqli_connect_error());
}
?>