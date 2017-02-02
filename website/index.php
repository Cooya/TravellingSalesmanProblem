<!--
14/04/2014

Little TSP Solver with TSP API
David Phan
-->

<?
if(file_exists("VDC") && fileperms("uploads/")==16895)
{
?>
<html>
<head>
	<title>Little TSP Solver with PHP Api for TSP</title>
	
	    <script type="text/javascript" src="//cdnjs.cloudflare.com/ajax/libs/jquery/2.1.0/jquery.min.js"></script>
	<link href="//netdna.bootstrapcdn.com/bootswatch/3.1.1/amelia/bootstrap.min.css" rel="stylesheet">
	
<!-- Latest compiled and minified JavaScript -->
<script src="//netdna.bootstrapcdn.com/bootstrap/3.1.1/js/bootstrap.min.js"></script>

	</head>
	
<body style="text-align:center;width:600px;margin:auto;">
<a href="./"><h1>Little TSP Solver using TSP API</h1></a>
<p>Please send a FULL_MATRIX TSPLIB, max 10MB<br />Time limit is set to 60 seconds, don't submit big instances (especially for Branch and Bound)</p>
<form role="form" enctype="multipart/form-data" action="process.php" method="POST">

  <div class="form-group">
    <label for="userfile">TSPLIB File input</label>
    <input type="file" id="userfile" name="userfile">
  </div>
    <div class="form-group">
    <label for="algorithm">Algorithm</label>
    <select class="form-control" name="algorithm" required>
    <option id="nn">Nearest Neighbour</option>
    <option id="mst">Minimum Spanning Tree</option>
    <option id="bb">Branch and Bound with Held Karp (optimum)</option>
    </select>
    </div>
  <button type="submit" class="btn btn-default">Submit</button>
  </form>
<pre style="margin:50px;">Using JQuery, Graph Dracula, Bootstrap and a Bootswatch Theme</pre>
</body>


</html>

<?
}
else 
{
echo "Please copy VDC executable (chmod 755) to this directory and check uploads permissions (should be 0777), it is ".fileperms("uploads/");
}
?>
