<!DOCTYPE hmtl>
<html>
	<head>
		<meta charset="utf-8"/>
		<link rel="stylesheet" href="css/style.css" />
		<title> Centrale de télésurveillance </title>
	</head>
	
		<body>
	
			<h1>Centrale dédiée à la télésurveillance</h1>
			<h4>Quentin Godbert - Edwige Neveu</h4>	
			<h5> 25 novembre 2014 </h5>
			<form action="cible.php" method="post"/>
			<p>
				A quelle fréquence souhaitez vous récupérer la température ?
				<input type="text" name="valeur" value="1" size="5" /> Hz
				<input type="submit" value="Valider"/>
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

<!aide : http://www.it-connect.fr/lire-et-ecrire-dans-un-fichier-en-php/>

<!readfile($file); >
