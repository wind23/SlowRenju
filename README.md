# SlowRenju

SlowRenju is a Gomoku/Renju engine, which supports the [new protocol](http://petr.lastovicka.sweb.cz/protocl2en.htm) of [Gomocup](https://gomocup.org/).

SlowRenju has been developed since January 2012, which is based on iteratively deepening alpha-beta pruning algorithm with a transposition table. SlowRenju has been participating in the Gomoku/Renju AI programming tournament [Gomocup](https://gomocup.org/) since 2013, which is one of the more famous computer Gomoku/Renju tournament now.

_Gomoku_, also known as _Five In a Row_, is a game playing on a 15×15 (or maybe larger) board. Two persons put their own stones on the crossing of the board alternatively, and the game ends with a player winning by creating a line of five stones horizontally, vertically or diagonally, or with a draw by the board filled up with stones. There are also some variants of Gomoku, among which the most famous one is _Renju_, in which the black player (the player who moves first) loses by creating a double-three, a double-four or an over-line.

SlowRenju supports the rules of Freestyle Gomoku, Standard Gomoku and Renju, on the board sizes between 5 and 20.

Homepage of SlowRenju: [https://www.wind23.com/gomokuai.html](https://www.wind23.com/gomokuai.html)

# Acknowledgement

Thanks to the friends who have been helping with the development of SlowRenju constantly:

 - [Kai Sun](https://www.kaisun.org/)
 - [Tao Tao](https://www.teaur.com/)
 - Rong Xiao


# License
SlowRenju is distributed under the GNU General Public License v3.0. See [here](https://www.gnu.org/licenses/gpl-3.0.en.html) for the detail of the license.

	(C) 2012-2019  Tianyi Hao
	
    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.
    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.
    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
