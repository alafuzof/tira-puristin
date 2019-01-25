# Viikkoraportti 2

## Mitä tein?

Tällä viikolla aloitin projektin koodin työstämisen. Melko paljon aikaa meni
käännösjärjestelmän (CMake), testauskirjaston (Google Test) ja dokumentaatio
järjestelmän (Doxygen) esivalmisteluun ja yhteensovittamiseen, mutta tämän
pitäisi olla projektitasolla kertainvestointi, eli jatkossa kehitys, testaus ja
dokumentointi pitäisi sujua nopeammin. Varsinaisen koodin osalta on nyt
toteutettu datan symbolitodennäköisyyksiä/-esiintymistä kirjaava FileAnalyzer,
joka on ensimmäinen osa Huffman-koodauksen toteuttamisen kannalta. Lisäksi
ohjelmalla on erittäin karkea komentorivikäyttöliittymä. Prioriteettijonon
osalta olen kerrannut kekototeutuksen pääpiirteet.

## Miten ohjelma on edistynyt?

Edellä mainittu FileAnalyzer on nyt toteutettu, dokumentoitu ja testattu ainakin
alustavasti. Muilta osin edistystä on tapahtunut lähinnä näkymättömissä
projektityökalujen alustamisen osalta.

## Mitä opin?

Opin paljon uutta C/C++ ohjelmointi-/projektiapuohjelmien käytöstä. Varsinkin
Google Test oli aivan uusi tuttavuus (CMake ja Doxygen entuudesta vähän
tutumpia). Uskoisin, että osaisin nyt tehdä käyttökelpoisen projektirangan C/C++
kielisille ohjelmille.

Prioriteettijonojen toteutuksesta lukiessani tutustuin myös binomi- ja
Fibonacci-kekoihin, mikä oli mielenkiintoista, vaikka toteutuksessa käyttäisin
peruskekoa.

## Mikä jäi epäselväksi tai tuotti vaikeuksia?

CMake käännösjärjestelmän kanssa oli aluksi vähän painimista, mutta kaikki siinä
kohdatut ongelmat ratkesivat lopulta.

## Mitä teen seuraavaksi?

Seuraavaksi on tarkoitus koodata alustava kekopohjainen prioriteettijono, jota
voi sitten käyttää Huffman-puun ja -koodin rakentamiseen. Huffman-koodauksen
osalta pitää vähän miettiä, kuinka tarvittavat bittioperaatiot saisi koodattua
mahdollisimman selkeästi.
