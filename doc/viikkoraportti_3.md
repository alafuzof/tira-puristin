# Viikkoraportti 3

## Mitä tein?

Tällä viikolla toteutin prioriteettijonon keon avulla PriorityQueue luokassa,
yksittäisi bittejä syöte- ja tulostevirroista lukevat/kirjoittavat BitWriter ja
BitReader luokat sekä karkean binääripuuluokan BinaryTree ja alustavan rungon
Huffman-koodaus luokalle HuffmanCode.

Lisäksi ohjaajan pyytämä testausraporttien näkyminen on nyt
toteutettu jatkuvan integroinnin avulla Travis CI sekä Codecov.io palveluiden
avulla - näiden ansiosta tiedän myös, että koodini vähintään kääntyy myös
muilla kun omalla koneellani.

## Miten ohjelma on edistynyt?

Ohjelma on edistynyt tällä viikolla hyvin. Uusina luokkina on nyt PriorityQueue,
BitWriter, BitReader, BinaryTree sekä HuffmanCode, joista kolme ensimmäistä on
lisäksi dokumentoitu ja testattu melko kattavasti. BinaryTree ja HuffmanCode
ovat työn alla ja näin ollen myös niiden testaus ja dokumentointi on vielä
keskeneräinen.

## Mitä opin?

Prioriteettijonon (l. keon) implementointi havainnollisti sen toimintaa hyvin.
Päädyin nimeämään ylläpito-operaatiot omasta mielestä kuvaavammalla tavalla,
varsinkin kun niitä tässä tapauksessa käytetään nimenomaan kekoehdon
ylläpitämiseen eikä esimmerkiksi avainarvon kasvattamiseen kuten Cormen et al.
tekstissä.

Opin lisäksi käyttämään Travis CI jatkuvaa integrointia sekä gcc-kääntäjän
testauskattavuustyökaluja (lcov) ja Codecov.io nimistä kattavuusraportointi
palvelua. Nämä olivat siis täysin uusia tuttavuuksia.

## Mikä jäi epäselväksi tai tuotti vaikeuksia?

Binääripuun muistinhallintaan ja erityisesti vapautukseen liittyy jonkinlainen
bugi, joka on nyt viimeisimmässä ohjelmaversiossa ohitettu yksinkertaisesti
jättämällä muisti vapauttamatta (tai no käyttöjärjestelmä vapauttaa sen ohjelman
suorituksen lopussa, mutta nykyinen ratkaisu on aika ruma).

## Mitä teen seuraavaksi?

Seuraavalla viikolla on tarkoitus saattaa Huffman-koodaus valmiiksi ja ehkä
siirtyä sanakirjakoodien vaatimiin tietorakenteisiin. Lisäksi
komentorivikäyttöliittymän erottaminen omaan luokkaansa selkeyttäisi koodia,
joten saatan laittaa siihen jonkin verran aikaa ensi viikolla. Binääripuun
muistiongelma olisi tarkoitus myös ratkaista ensi viikolla.
