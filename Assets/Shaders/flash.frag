#version 130

// Uniforms : variables passées depuis le code C++
uniform sampler2D texture;    // L'image du sprite
uniform vec4 flashColor;      // La couleur du flash (ex: blanc)
uniform float flashIntensity; // Intensité du flash (0.0 = normal, 1.0 = tout blanc)

void main()
{
    // On récupère la couleur du pixel de l'image à cet endroit
    vec4 pixelColor = texture2D(texture, gl_TexCoord[0].xy);

    // On mélange la couleur du pixel avec la couleur du flash selon l'intensité
    // mix(couleurA, couleurB, pourcentage)
    vec4 finalColor = mix(pixelColor, flashColor, flashIntensity);

    // On s'assure que la transparence (alpha) reste celle de l'image d'origine
    // Sinon le rectangle transparent autour du sprite deviendrait blanc aussi !
    finalColor.a = pixelColor.a * gl_Color.a;

    // C'est la couleur finale affichée à l'écran
    gl_FragColor = finalColor;
}