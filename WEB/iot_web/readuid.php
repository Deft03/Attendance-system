<?php
header('Content-Type: application/json');

// dang nhap vao database
include("config.php");

// Doc gia tri RGB tu database
$sql = "select * from uid_table";
$result = mysqli_query($conn,$sql);
$UIDtable = array();
foreach ($result as $row){
    $UIDtable[] = $row;
}
mysqli_close($conn);
echo json_encode($UIDtable);

?>