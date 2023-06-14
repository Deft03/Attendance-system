<?php
     // doc user input
    $id = $_POST["getUID"]; ;
    $name = $_POST["name"];
    $tuoi = $_POST["age"];
    $mssv = $_POST["MSSV"];
    $lop = $_POST["lop"];
    include("config.php");
    $sql = "insert into datastudent(id,Ten,Tuoi,MSSV,Lop) values('$id','$name','$tuoi','$mssv','$lop')";
    $sql1 = "UPDATE datastudent SET timein = null WHERE id = $id";
    //$sql2 = "UPDATE uid_table SET id= null WHERE 1";
    mysqli_query($conn, $sql);
    mysqli_query($conn, $sql1);
    //mysqli_query($conn, $sql2);
    mysqli_close($conn);
?> 