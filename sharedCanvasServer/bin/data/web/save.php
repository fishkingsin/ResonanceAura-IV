<?php
$id = $_POST['id'];
$img = $_POST['img'];
$img = str_replace('data:image/jpeg;base64,', '', $img);
$img = str_replace(' ', '+', $img);
$data = base64_decode($img);
$file = $_SERVER['DOCUMENT_ROOT'] . '/images/drawing/'.$id.'.jpg';
if(file_put_contents($file, $data)!== false) {
	echo "success";
} else {
	echo "fail";
}

?>