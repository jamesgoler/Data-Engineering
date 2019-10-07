<?php
header('Access-Control-Allow-Origin: *');
header('Access-Control-Allow-Methods: *');
header("Content-Type:text/html; charset=utf-8");
ini_set("default_charset", "utf8");
ini_set("memory_limit", "-1");

$page_number = $_GET["page_number"];

process($page_number);

function process($page_number) {
	$result = read($page_number);
	$num = count($result);

	$return_list = array();

	for ($i = 0; $i < $num; $i++) {
		$item = explode("\t", $result[$i]);
		$return_list[$i] = array("content"=>$item[1], "count"=>$item[0]);
	}
	echo json_encode($return_list);
}

function read($page_number) {
	$file_path = "../../data/tmp/".strval($page_number);
	$command = "cat '$file_path'";
	$result = array();
	exec($command, $result);
	return $result;
}