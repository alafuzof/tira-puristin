# Tiedon pakkaus

## Aihe

Toteutan pakkausohjelman, joka pystyy häviöttömästi pakkamaan ja purkamaan
tiedostoja Huffman-koodauksen ja Lempel-Ziv-Welch (LZW) tyylisen 
sanakirja-koodauksen avulla. 

## Algoritmit ja tietorakenteet

Huffman-koodaus on entropiakoodeja ja näin ollen vaatii tiedon syötteessä 
esiintyvien merkkien todennäköisyyksistä. Todennäköisyyksien esittämiseen 
riittää yksinkertainen taulukko.

Huffman-koodauksen aikana rakennetaan nk. Huffman-puu, joka on tietty 
merkkitodennäköisyyksien määrittämä binääripuu. Binääripuurakenteen lisäksi
Huffman-puun rakentaminen edellyttää prioriteettijonoa.

LZW-koodauksen tehokas toteutus edellyttää nopeaa merkkijonopohjaista 
indeksöintiä. Tähän tarkoitukseen toteutan hajautustaulun.

## Ongelma ja lähestymistapa

Tiedon pakkaus on mielenkiintoinen aihe, koska se on niin perustavanlaatuinen
ongelma ja se liittyy niin moneen tietojenkäsittelytieteen osa-alueeseen 
(algoritmit, ohjelmistosuunnittelu, koneoppiminen, jne.). Valitsin Huffman- ja
LZW-koodauksen, koska nämä ovat algoritmit ovat melkein kaikkien yleisesti 
käytössä olevien pakkausmenetelmien taustalla.

Huffman-puu voidaan rakentaa usealla eri tavalla [1], mutta näistä 
prioriteettijono on ehkä intuitiivisesti ymmärrettävin, joten valitsin sen.

LZW-koodauksen sanakirjahaut voidaan toteuttaa usealla tavalla [2], mutta 
toteutus hajautustaulun avulla pitäisi olla melko tehokas.

## Syötteet

Ohjelma toteutetaan komentorivikäyttöliittymällä, jossa pakattava/purettava
data valitaan tiedostonimen avulla. 

## Aika ja tilavaativuudet

Symbolitodennäköisyyksien laskeminen edellyttää syötteen läpikäymistä ja on
näin ollen aikavaativuudeltaan O(n), missä n on syötteen pituus. 
Todennäköisyyksien tallentaminen vaatii O(m) tilaa, missä n on symbolien 
lukumäärä (esim. 2^8=256 tavukoodauksessa).

Huffman-puun rakentaminen prioriteettijonon avulla vie O(m log m) aikaa ja O(m)
tilaa [1].  

LZW-koodauksessa syöte käydään merkki kerrallaan ja kunkin kohdalla tehdään haku
hajautustauluun sekä mahdollinen lisäys: hajautustaulun haku- ja lisäysoperatiot
ovat O(1), eli LZW-koodaus kokonaisuudessaan pitäisi tapahtua O(n) ajassa, missä 
n on syötteen merkkien lukumäärä. Koodauksessa käytettävän sanakirjan koko on 
kiinteästi määritelty (esim. 4096 jäsentä), eli tilavaativuus on O(1).

## Lähteet

[1] Wikipedia. Huffman Coding. https://en.wikipedia.org/wiki/Huffman_coding

[2] P.M. Nishad & R.M. Chezian. Behavioral Study of Data Structures on Lempel 
Ziv Welch (LZW) Data Compression Algorithm and ITS Computational Complexity.
https://doi.org/10.1109/ICICA.2014.64
