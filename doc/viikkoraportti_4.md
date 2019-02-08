# Viikkoraportti 4

## Mitä tein?

Tällä viikolla kurssin ulkopuoliset menot rajoittivat työskentelyä ja yleistä
edistymistä. Koodin testausta on kuitenkin parannettu huomattavasti
(testikattavuus on noussut >10%).

Binääripuille on nyt toteutettu tallennustoiminnallisuus ja bittiluku ja
-kirjoitus luokkien BitReader ja BitWriter toiminnallisuutta on laajennettu
merkittävästi.

Lisäksi Huffman-koodaus on periaatteessa nyt toteutettu ja integroitu
käyttöliittymään, mutta jonkinlainen bugi Huffman-puun tiedostoonkirjoittamisessa toistaiseksi estää oikein toimivan pakkaus-purku-silmukan. Valitettavasti tämä
esti myös suorituskykytestauksen aloittamisen.

## Miten ohjelma on edistynyt?

Huffman-koodauksen osalta toiminnallisuus on nyt "valmis", mutta buginen.

## Mitä opin?

Tällä viikolla ei ollut mitään merkittävää uutta opittavaa.

## Mikä jäi epäselväksi tai tuotti vaikeuksia?

Binääripuun tallennuksessa tai lukemisessa on bugi, joka täytyy selvittää.

## Mitä teen seuraavaksi?

Seuraavalla viikolla on tarkoitus viilata Huffman-koodin bugit pois ja aloittaa
sen suorituskykytestaus. Lisäksi olisi tarkoitus aloittaa sanakirjakoodien
vaatimien tietorakenteiden koodaus. Komentorivikäyttöliittymä pitäisi edelleen
erottaa omaan luokkaansa.
