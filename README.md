* Les controles :
 * controles de base pour tout ce qui est zoom/dezoom
 * rotation de la camera à la souris, il faut cliquer sur une fenêtre pour capturer le curseur
 * echap permet de libérer le curseur
 * tab quitte toutes les fenêtres

* L'alternance entre les saisons se fait toutes les 120 secondes, un bouton dans une fenêtre supplémentaire (qui sert de serveur) permet également de changer la saison.

* Il neige en hiver, la neige reste là où elle est tombée, c'est géré avec une matrice de la taille de la heightmap, et à chaque case de cette matrice correspond une hauteur de neige.

* Au printemps des fleurs poussent.

* En été le sol jaunit et la neige des sommets disparait.

* En automne il pleut, lorsqu'une goutte de pluie touche le sol, le niveau de l'eau monte, il y a également régulièrement des eclairs.

* Lors d'un changement de saison, l'ancienne saison ne disparait pas directement, la neige fond au fur et à mesure, le niveau de l'eau baisse, l'herbe redevient verte et les fleurs fannent.

* Toutes les scènes sont éclairées, cela permet d'avoir un rendu réaliste, de bien voire les reliefs et d'avoir une eau cohérente.

* Les gouttes d'eau, les fleurs et les flocons de neige sont gérés par un système de pool, dans laquelle on pioche et on remet les éléments à la fin de leur utilisation.

* Les triangles du terrain sont généres pour être utilisés comme GL_TRIANGLE_STRIP, cela améliore grandement les performances, mais cela rend le calcul des normales plus complexes, mais rien d'insurmontable, tout est précalculé et stocké dans un tableau pour gagner en performances.

* Pour ce qui est des bonus, tout est évidemment parallélisé, il y a également une accumulation des particules au sol : la neige reste et le niveau de l'eau augmente, cela peut même créer des rivières dans les vallées.
