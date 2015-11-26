<!DOCTYPE hmtl>
<html>
	<head>
		<meta charset="utf-8"/>
		<link rel="stylesheet" href="style.css" />
		<title>Frequence</title>
	</head>
	
	<body>
		<h1> Valeur de la fréquence </h1>

		<?php 
			//récupération de la fréquence
			unlink('frequency.txt');
			$frequence = $_POST['valeur'];
			$file ="frequency.txt";
			
			$file = fopen("$file",'a');
			# Ecriture
			
			fwrite($fileopen,"$frequence ");
			
			fclose($fileopen);

			echo "Frenquence : $frequence simulation de temperature par secondes"
		 ?>

		<br/> 
		</br>
		<a href="index.php"> Retour au centre de télésurveillance </a></li>

	</body>

</html>