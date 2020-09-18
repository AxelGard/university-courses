package se.liu.ida.axega544.tddd78.tetris;

import java.util.Random;

class TetrominoMaker {

    private int getNumberOfTypes() {
        return SquareType.values().length ;
    }

    private SquareType[][] squares = null;
    private int lenOfPoly = 0;

    private Poly getPoly(int n) {

        switch (n) {
            case 0:
                return I();
            case 1:
                return J();
            case 2:
                return L();
            case 3:
                return O();
            case 4:
                return S();
            case 5:
                return T();
            case 6:
                return Z();
            default:
                throw new IllegalArgumentException("Invalid index: " + n);
        }

    }

    public Poly getRandomPoly(){
        Random rnd = new Random();
        return getPoly(rnd.nextInt(getNumberOfTypes()-2));
    }


    private Poly I() {
        squares = new SquareType[4][4];
        lenOfPoly = 4;
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                squares[i][j] = SquareType.EMPTY;
                squares[1][j] = SquareType.I;
            }
        }
        return new Poly(squares);
    }


    private Poly J() {
        squares = new SquareType[3][3];
        lenOfPoly = 3;
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) squares[i][j] = SquareType.EMPTY;
        }
        squares[0][1] = SquareType.J;
        squares[1][1] = SquareType.J;
        squares[2][1] = SquareType.J;
        squares[2][0] = SquareType.J;
        return new Poly(squares);
    }


    private Poly L() {
        squares = new SquareType[3][3];
        lenOfPoly = 3;
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                squares[i][j] = SquareType.EMPTY;
            }
        }
        squares[0][1] = SquareType.L;
        squares[1][1] = SquareType.L;
        squares[2][1] = SquareType.L;
        squares[2][2] = SquareType.L;
        return new Poly(squares);
    }


    private Poly O() {
        squares = new SquareType[2][2];
        lenOfPoly = 2;
        for (int i = 0; i < 2; i++) {
            for (int j = 0; j < 2; j++) {
                squares[i][j] = SquareType.O;
            }
        }
        return new Poly(squares);
    }


    private Poly S() {
        squares = new SquareType[3][3];
        lenOfPoly = 3;
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                squares[i][j] = SquareType.EMPTY;
            }
        }
        squares[0][1] = SquareType.S;
        squares[0][2] = SquareType.S;
        squares[1][0] = SquareType.S;
        squares[1][1] = SquareType.S;
        return new Poly(squares);
    }


    private Poly T() {
        squares = new SquareType[3][3];
        lenOfPoly = 3;
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                squares[i][j] = SquareType.EMPTY;
                squares[1][j] = SquareType.T;
                squares[0][1] = SquareType.T;

            }
        }
        return new Poly(squares);
    }


    private Poly Z() {
        squares = new SquareType[3][3];
        lenOfPoly = 3;
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                squares[i][j] = SquareType.EMPTY;

            }
        }
        squares[0][0] = SquareType.Z;
        squares[0][1] = SquareType.Z;
        squares[1][1] = SquareType.Z;
        squares[1][2] = SquareType.Z;
        return new Poly(squares);
    }

// --Commented out by Inspection START (2019-02-05 10:51):
//    public int getLenOfPoly() {
//        return lenOfPoly;
//    }
// --Commented out by Inspection STOP (2019-02-05 10:51)
}


