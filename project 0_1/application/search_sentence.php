<?php
header('Access-Control-Allow-Origin: *');
header('Access-Control-Allow-Methods: *');
header("Content-Type:text/html; charset=utf-8");
ini_set("default_charset", "utf8");
ini_set("memory_limit", "-1");

$keyword = $_GET["keyword"];

process($keyword);

function process($keyword) {
	$result = search($keyword);
	$num = count($result);

	$return_list = array();

	for ($i = 0; $i < $num; $i++) {
		$item = explode("\t", $result[$i]);
		$return_list[$i] = array("content"=>$item[1], "count"=>$item[0]);
	}
	echo json_encode($return_list);
}

function search($keyword) {
	$file_path = "../data/result.txt";
	$command = "grep '$keyword' '$file_path'";
	$result = array();
	exec($command, $result);
	return $result;
}