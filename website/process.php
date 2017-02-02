	<html>
  <head>
  	<title>Little PHP Api for TSP by David P. Process Page</title>
    <!-- jQuery -->
    <script type="text/javascript" src="js/jquery-1.4.2.min.js"></script>
    <!--  The Raphael JavaScript library for vector graphics display  -->
    <script type="text/javascript" src="js/raphael-min.js"></script>
    <!--  Dracula  -->
    <!--  An extension of Raphael for connecting shapes -->
    <script type="text/javascript" src="js/dracula_graffle.js"></script>
    <!--  Graphs  -->
    <script type="text/javascript" src="js/dracula_graph.js"></script>
    <script type="text/javascript" src="js/dracula_algorithms.js"></script>

    <script type="text/javascript" src="js/layoutfix.js"></script>

	    <script type="text/javascript" src="//cdnjs.cloudflare.com/ajax/libs/jquery/2.1.0/jquery.min.js"></script>
	<link href="//netdna.bootstrapcdn.com/bootswatch/3.1.1/amelia/bootstrap.min.css" rel="stylesheet">

<!-- Latest compiled and minified JavaScript -->
<script src="//netdna.bootstrapcdn.com/bootstrap/3.1.1/js/bootstrap.min.js"></script>


<?php
/* Traitement du formulaire envoyé via index.php */

/* Traitement du fichier */
$uploaddir = dirname(__FILE__).'/uploads/';
$filename = preg_replace("/[^a-zA-Z0-9]+/", "_", basename($_FILES['userfile']['name']));
$uploadfile = $uploaddir . md5(mt_rand(10000000,999999999)). '_' . $filename;

if (move_uploaded_file($_FILES['userfile']['tmp_name'], $uploadfile)) // si le fichier tsp a bien été envoyé
{

	/* 	Recupere la bibliotheque */
	require_once("libjson.php");


	set_time_limit(60);


	/* 	Nous traitons le fichier avec TSP en mode API, selon ce que l'utilisateur a choisi */
	if($_POST['algorithm']=="Nearest Neighbour") // l'utilisateur a choisi Nearest Neighbour
		{
		$output=execNN($uploadfile);
		$outputfile=getOutputNN($uploadfile);
	}
	if($_POST['algorithm']=="Minimum Spanning Tree") // l'utilisateur a choisi MST
		{
		$output=execMST($uploadfile);//query= "./VDC -api -mst ".$uploadfile;
		$outputfile=getOutputMST($uploadfile); //$uploadfile.'_mst.json';
	} if($_POST['algorithm']=="Branch and Bound with Held Karp (optimum)") // l'utilisateur a choisi BB
		{
		$query=execBB($uploadfile); // "./VDC -api -bb ".$uploadfile;
		$outputfile=getOutputBB($uploadfile); //$uploadfile.'_opt.json';

	}

	//set_time_limit(60);
	//$output=shell_exec($query);
	//echo $outputfile;

	if(file_exists($outputfile)) // si le fichier json sorti par VDC
		{
		$fp=fopen($outputfile, 'r');
		$json=fread($fp, filesize($outputfile));
		fclose($fp);
		$json=json_decode($json, TRUE);


		//echo(var_dump($json));


?>

<script type="text/javascript">

var redraw;
var redrawRandom;

window.onload = function() {
    var width = $(document).width() - 200;
    var height = $(document).height() ;

var g = new Graph();



<?php

		// creating nodes

		createNodesJs($json);

		// creating edges
		createEdgesJs($json);


?>

/* layout the graph using the Spring layout implementation */
var layouter = new Graph.Layout.Fixed(g);
layouter.layout();

/* draw the graph using the RaphaelJS draw implementation */
var renderer = new Graph.Renderer.Raphael('canvas', g, width, height);
renderer.draw();



    redraw = function() {
    	var layouter= new Graph.Layout.Fixed(g);
        layouter.layout();
        renderer.draw();
    };
    redrawRandom = function() {
    	var layouter = new Graph.Layout.Spring(g);
    	layouter.layout();
    	renderer.draw();
    };

};



</script>
  </head>
  <body style="text-align:center;margin:auto;">
  <?php
		echo '<a href="./"><h1>Little TSP Solver using TSP API</h1></a>';
		echo '<div id="result">'.PHP_EOL;
		printResult($json);
		echo '</div>';

		echo'    <div id="canvas" style="margin-top:50px;margin-left:50px;margin-right:50px;background-color:white; border-radius:20px;"></div>
    <button style="margin:10px" type="button" class="btn btn-default" id="redraw" onclick="redraw();">Redraw</button>';

		if(isPos($json))
		{
			echo '<button style="margin:10px" type="button" class="btn btn-default" id="redraw" onclick="redrawRandom();">Redraw with random Pos</button>';
			echo '<br />';

		}

	}
	else { // le fichier output de VDC  n'existe pas => mauvais tsp, ou erreur de VDC
		echo '  </head>
  <body style="width:600px;margin:auto;text-align:center;"><a href="./"><h1>Little TSP Solver using TSP API</h1></a><br /><p class="bg-danger">ERROR : PLEASE CHECK YOUR TSP FILE,  Here is the output of TSP API with your submitted file ` '.htmlspecialchars($_FILES['userfile']['name']).'` </p><br/> <pre>'.$output;
		//echo "<br />";
		//print_r($_FILES);
		echo '</pre>';
	}

}

else {// fichier n'a pas ete envoye correctement
	echo "Upload failed, check hdd space and chmod 777 uploads/";
}





/*
echo "</p>";
echo '<pre>';
echo 'Here is some more debugging info:';
print_r($_FILES);
print "</pre>";
*/


?>
<p>
<a href="./"><button style="margin:10px;" type="button" class="btn btn-default btn-lg">Return</button>
</a>
</p>
  </body>
</html>
