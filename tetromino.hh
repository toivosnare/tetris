/* TIE-02201/TIE-02207 SPRING 2020
 * Program author
 * Name: Toivo Snåre
 * userID: snare
 * E-Mail: toivo.snare@tuni.fi
 *
 * Class: Tetromino
 * Class that represents a Tetris piece with shape, color, position and
 * rotation.
 */

#ifndef TETROMINO_HH
#define TETROMINO_HH

#include "pixelview.hh"
#include <QVector2D>
#include <QBrush>

class Tetromino {
public:
	Tetromino(const QVector<QVector2D>& shape, const QVector2D& position,
			  const QBrush& color);
	// Displays itself on the pixelView screen.
	void render(PixelView* screen) const;
	// Returns a list of coordinates of the cells according to the shape,
	// position and rotation.
	QVector<QVector2D> cells() const;
    QVector<QVector2D> cells(const QVector2D& position, int rotation) const;
	QVector2D getPosition() const;
	int getRotation() const;
	QBrush getColor() const;
	void setPosition(const QVector2D& newPosition);
    void setRotation(int newRotation);
private:
	QVector<QVector2D> shape;
	QVector2D position;
	QBrush color;
	int rotation;
};

#endif // TETROMINO_HH
