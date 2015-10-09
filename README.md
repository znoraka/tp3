Les paramêtres etat, ss, rotX et rotY servent respectivement à changer l'affichage de la heightmap (couleurs, points...), à scale la caméra et à la rotation selon les axes X et Y de la camera.

Plus la fréquence dimine, plus l'update se fait à intervalle écarté, on peut cependant noter que même si le rafraichissement des différentes fenêtres est différent, la vitesse de rotation de la heightmap reste la même. C'est du au fait que la rotation est fonction du temps passé et non pas du nombre d'updates.

La classe GameWindow hérite de OpenGLWindow pour tous les avantages que l'héritage apporte. Les fonctions de base sont contenues dans la classe mère, qui est spécialisée dans la classe fille, ici GameWindow spécifie les évènement claviers par exemple. La classe OpenGLWindow est un modèle et GameWindow gère la mécanique.
