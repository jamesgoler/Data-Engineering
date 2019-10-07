<?php
header('Access-Control-Allow-Origin: *');
header('Access-Control-Allow-Methods: *');
header("Content-Type:text/html; charset=utf-8");
ini_set("default_charset", "utf8");
ini_set("memory_limit", "-1");

$keyword = $_GET["keyword"];

process($keyword);

function process($keyword) {
	clear_prev();

	$result = search($keyword);
	
	segment_page($result);

	echo "done";
}

function search($keyword) {
	$file_path = "../../data/result.txt";
	$command = "grep '$keyword' '$file_path'";
	$result = array();
	exec($command, $result);
	return $result;
}

function clear_prev() {
	$file_path = "../../data/tmp/*";
    $files = glob($file_path, GLOB_MARK);
    foreach ($files as $file) {
		unlink($file);
    }
}

function segment_page($result) {
	$num = count($result);

	$page_number = 0;
	for ($i = 0; $i < $num; $i++) {
		$filename = $page_number + 1;
		$fp = fopen("../../data/tmp/".strval($filename), "w");
		for ($j = 0; $j < 100 && $i < $num; $j++, $i++) {
			fprintf($fp, "%s\n", $result[$i]);
		}
		$page_number++;
		fclose($fp);
	}
}