<!DOCTYPE hmtl>
<html>
	<head>
		<meta charset="utf-8"/>
		<link rel="stylesheet" href="css/style.css" />
		<title> Valeur de la fréquence choisie</title>
	</head>
	
	<body>
		<h1> Valeur de la fréquence </h1>

		<?php 
			//récupération de la fréquence
			unlink('frequency.txt');
			$frequence=$_POST['valeur'];
			$file ="frequency.txt";
			# Ouverture en mode écriture
			$fileopen=(fopen("$file",'a'));
			# Ecriture de "Début du fichier" dansle fichier texte
			fwrite($fileopen,"$frequence ");
			# On ferme le fichier proprement
			fclose($fileopen);

			echo "Maintenant la fréquence est celle que vous avez choisi : $frequence capture(s) par secondes"
		 ?>

		<br/> 
		</br>
		<a href="index.php"> Retour au centre de télésurveillance </a></li>

	</body>

</html>