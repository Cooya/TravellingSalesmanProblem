<?php

/*
14/04/2014 - David Phan
La Bibliotheque JSON est un ensemble de fonctions destinée à récupérer les informations
des fichiers JSON générés par le projet C "TSP (Traveling Salesman Problem) Solver"
*/

/* Recupere le nom du fichier sorti par TSP en mode API pour MST et $uploadfile */
function getOutputMST($uploadfile)
{
	$outputfile=$uploadfile.'_mst.json';
	return $outputfile;
}

/* Execute TSP en mode API avec l'algorithme MST et $uploadfile */
function execMST($uploadfile)
{
	$query= "./VDC -api -mst ".$uploadfile;
	$output=shell_exec($query);
	return $output;
}


/* Recupere le nom du fichier sorti par TSP en mode API pour Branch and bound et $uploadfile */
function getOutputBB($uploadfile)
{
	$outputfile=$uploadfile.'_opt.json';
	return $outputfile;
}

/* Execute TSP en mode API avec l'algorithme Branch and Bound et $uploadfile */
function execBB($uploadfile)
{
	$query= "./VDC -api -bbrhk ".$uploadfile;
	$output=shell_exec($query);
	return $output;
}



/* Recupere le nom du fichier sorti par TSP en mode API pour Nearest Neighbour et $uploadfile */
function getOutputNN($uploadfile)
{
	$outputfile=$uploadfile.'_nn.json';
	return $outputfile;
}

/* Execute TSP en mode API avec l'algorithme Nearest Neighbour et $uploadfile */
function execNN($uploadfile)
{
	$query= "./VDC -api -nn ".$uploadfile;
	$output=shell_exec($query);
	return $output;
}
/* Recupere le nombre de point dans la Map */
function getSize($json)
{
	return $json["tsp"]["nb"];
}


/* Recupere la longueur du chemin le plus court trouve */
function getPathLen($json)
{
	return $json["tsp"]["result"]["path_len"];
}

/* Recupere la longueur totale du chemin le plus court trouvé */
function getResultLength($json)
{
	return $json["tsp"]["result"]["length"];
}

/* Recupere le nom de l'algorithme utilisé */
function getAlgoName($json)
{
	return $json["tsp"]["result"]["algorithm"];
}

/* 
Ecrit le code source javascript pour afficher les Node (les points) du graphe,
si la Map a les points cartesiens, alors il dessine avec ses coordonnées, sinon il les dispose aléatoirement
*/
function createNodesJs($json)
{
	for($i=0;$i<getSize($json);$i++)
	{
		echo 'g.addNode("'.($i+1).'", {label:'.($i+1);

		//if position
		if(isPos($json))
		{
			echo ',x:'.getX($json,$i).',y:'.getY($json,$i);
		}

		echo '});'.PHP_EOL;
	}
}

/* Affiche les données liées au résultat : Taille de l'instance, Longueur la plus courte trouvée, Le chemin (avec les distances) */
function printResult($json)
{
	echo "Result for ".htmlspecialchars($_FILES['userfile']['name'])."<br />\n";
	echo "Map size is : " . getSize($json)."<br />\n";
	echo "Best length found using ".getAlgoName($json). " is ".getResultLength($json)."<br />\n";
	echo "Tour is : ";
	$path=getPath($json);
	$dists_path=getDistsPath($json);
	for($i=0;$i<getPathLen($json);$i++)
	{
		echo '<span style="font-size:20px; ;">'.$path[$i].'</span>';

		if($i!=getPathLen($json)-1)
		{
			echo " -<span style=\"font-size:10px;\">". substr($dists_path[$i+1],0,4) . "</span>-> ";

		}
	}
	echo "<br />";
}

/* Ajoute le code JS pour définir les connections entre les Nodes pour dessiner le chemin le plus court */
function createEdgesJs($json)
{
	$resP=getPath($json);
	$resDP=getDistsPath($json);
	for($i=0;$i<(getPathLen($json)-1);$i++)
	{
		echo 'g.addEdge("'.$resP[$i].'", "'.$resP[$i+1].'", {directed:true , label:'.$resDP[$i+1].'});'.PHP_EOL;
	}
}


/* Retourne si l'instance Map est définit par des City positionnés */
function isPos($json)
{
	return $json["tsp"]["isPos"]=="true";
}
/* Affiche les points */
function printPos($json)
{
if(isPos($json))
{
	echo "Points of Instance : <br />\n";
	for($i=0;$i<getSize($json);$i++)
	{
		echo 'Point '.($i+1).'('.getX($json,$i).','.getY($json,$i).')<br />';
	}
}
else
{
	echo 'the instance doesnt have point, cant print pos';
}
}
/* Recupere l'abscisse du point $i */
function getX($json, $i)
{
	return $json["tsp"]["POS"][$i]["x"];
}

/* Recupere l'ordonnée du point $j */
function getY($json, $i)
{
	return $json["tsp"]["POS"][$i]["y"];
}

/* Recupere le tableau de chemin le plus court */
function getPath($json)
{
	$arr=array();
	for($i=0;$i<getPathLen($json);$i++)
	{
		$arr[$i]=$json["tsp"]["result"]["path"][$i];
	}
	return $arr;
}

/* Recupere le tableau des distances du chemin le plus court */
function getDistsPath($json)
{
	$arr=array();
	for($i=0;$i<getPathLen($json);$i++)
	{
		$arr[$i]=$json["tsp"]["result"]["dists_path"][$i];
	}
	return $arr;
}

/* Affiche la Matrice des distances */
function print2DMAT($json)
{
	for($i=0;$i<getSize($json);$i++)
	{
		for($j=0;$j<getSize($json);$j++)
		{
			echo $json["tsp"]["2DMAT"][$i][$j]." "; //->2DMAT->$i->$j." ";
		}
		echo "<br />\n";
	}

}

/* Retourne la matrice des distances */
function get2DMAT($json)
{
	$arr=array(array());
	for($i=0;$i<getSize($json);$i++)
	{
		for($j=0;$j<getSize($json);$j++)
		{
			$arr[$i][$j]=$json["tsp"]["2DMAT"][$i][$j]; //." "; //->2DMAT->$i->$j." ";
		}
		//echo "<br />\n";
	}

	return $arr;
}


?>
