<!DOCTYPE hmtl>
<html>
	<head>
		<meta charset="utf-8"/>
		<link rel="stylesheet" href="css/style.css" />
		<title> Centrale de télésurveillance </title>
	</head>
	
		<body>
	
			<h1>Centrale de télésurveillance</h1>
			<h3>Mehdi Lakbir - Morgan Perre</h3>	
			<h5> 27 novembre 2015 </h5>
			
			
			<form action="freq.php" method="post"/>
			
			<p>
				Fréquence de recupération de température ?
				
				<input type="text" name="valeur" value="1" size="4" /> Hz
				
				<input type="submit" value="Envoyer"/>
			</p>

			<p>
				<?php 

					//affichage de la trame sur le site
					for($i=0; $i<5; $i++)
					{
						echo "Trames envoyées : ";
						$file ="liste_trame.txt";
						# On met dans la variable (tableau $read) le contenu du fichier
						$read=file($file);
						#On affiche ensuite le fichier ligne par ligne (pour un fichier de deux lignes)
						//echo $read[$i]; //si on veut afficher ligne par ligne sans effacer à chaque réécriture
						readfile($file); //pour écrire le fichier entier "pas top"						
					}
				?>
			</p>
		</body>

</html>
