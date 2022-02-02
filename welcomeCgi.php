<html>
	<head>
		<style>
			h1 {
				margin-top: 80;
				color: maroon;
				font-family: fantasy;
				text-align: center;
				font-size: 340%;
			}
			img {
				margin-top: 60;
				display: block;
				margin-left: auto;
				margin-right: auto;
				width: 50%;
			}
		</style>
		<title>Happy Day</title>
		<link rel="icon" type="image/x-icon" href="cat_face.png">
	</head>
	<body style="background-color: lightsalmon;">
		<h1>Welcome <?php echo $_POST["fname"] . " " . $_POST["lname"]; ?> &#128081</h1>
		<img src="newcat.jpg" alt="new me">
	</body>
</html>