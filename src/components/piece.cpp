#include "components/piece.hpp"

Piece::Piece(int owner_id) {
    owner_id_ = owner_id;
}

std::vector<Piece::Point> Piece::get_points() const {
    return points_; 
}

Piece::piece_type Piece::get_piece_type() const {
    return piece_type_;
}

int Piece::get_owner_id() const {
    return owner_id_;
}

void Piece::rotate(rotation rot) {
    // Update the piece's current rotation.
    if (rot == Piece::rotation::clockwise) {
        rotation_ += 90;
        // Keep rotation within [0,360) degrees.
        if (rotation_ == 360) {
            rotation_ = 0;
        }
    } else {
        // Change 0 to 360 to keep rotation in positive degrees.
        if (rotation_ == 0) {
            rotation_ = 360;
        }
        rotation_ -= 90;
    }

    // Squares stay the same when rotated by 90 degrees, so it's simpler
    // to not change the order of the points by not rotating at all.
    if (piece_type_ != Piece::piece_type::square) {
        // Update the points based on the rotation.
        if (rot == Piece::rotation::clockwise) {
            // For clockwise, replace (x,y) with (y,-x).
            for (Piece::Point& point : points_) {
                int x = point.x;
                int y = point.y;
                point.x = y;
                point.y = -x;
            }
        } else {
            // For counterclockwise, replace (x,y) with (-y,x).
            for (Piece::Point& point : points_) {
                int x = point.x;
                int y = point.y;
                point.x = -y;
                point.y = x;
            }
        }
    }
}
