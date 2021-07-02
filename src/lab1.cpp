#include <iostream>
#include <vector>
#include <cmath>

using namespace std;

class Point{
public:
    float x;
    float y;

    Point() = default;

    Point(float x, float y): x(x), y(y){}

    Point(const Point& other):x(other.x), y(other.y){}

    Point operator = (Point const& other){
        x = other.x;
        y = other.y;
        return *this;
    }

    Point operator + (Point const& other) const{
        Point p(x + other.x, y + other.y);
        return p;
    }

    Point operator - (Point const& other) const{
        Point p(x - other.x, y - other.y);
        return p;
    }

    Point operator * (float const& scalar) const{
        Point p(x * scalar, y * scalar);
        return p;
    }

    Point operator / (float const& scalar) const{
        Point p(x / scalar, y / scalar);
        return p;
    }

    float distance(Point const& other) const{
        return sqrt(pow(x-other.x, 2.0f) + pow(y-other.y, 2.0f));
    }

    void move(float x, float y){
        this->x += x;
        this->y += y;
    }

    void scale(float xScale, float yScale){
        this->x *= xScale;
        this->y *= yScale;
    }

    void draw(char** field, int width, int height, char character) const{
        field[(int)y][(int)x] = character;
    }
};

std::ostream& operator<<(std::ostream& os, Point& point){
    return os << "(" << point.x << ", " << point.y << ")";
}

class Polyline{
protected:
    vector<Point> points;
public:
    explicit Polyline(vector<Point> const& _points){
        points.insert(points.begin(), _points.begin(), _points.end());
    }

    Polyline(Polyline const& other){
        points.insert(points.begin(), other.points.begin(), other.points.end());
    }

    Polyline& operator = (Polyline const& other){
        points.insert(points.begin(), other.points.begin(), other.points.end());
        return *this;
    }

    virtual float length() const{
        if(points.empty())
            return 0;
        float perimeter = 0;
        size_t length = points.size();
        for(size_t i = 1; i < length; i++)
            perimeter += points[i-1].distance(points[i]);
        return perimeter;
    }

    std::ostream& print(std::ostream& os) {
        int length = points.size();
        if(length == 0)
            return os;
        os << points[0];
        for(int i = 1; i < length; i++){
            os << "-" << points[i];
        }
        return os;
    }

    Polyline& move(float x, float y){
        for(Point& point : points){
            point.move(x, y);
        }
        return *this;
    }

    Polyline& scale(float xScale, float yScale){
        for(Point& point : points){
            point.scale(xScale, yScale);
        }
        return *this;
    }

    virtual void draw(char** field, int const width, int const height) const{
        cout << "Draw Polyline" << endl;
        drawPoints(field, width, height, 'L');
        drawPointsBetween(field, width, height, 'L');
    }
protected:
    Polyline() = default;
    void drawPoints(char** field, int const width, int const height, char character) const{
        for(Point const& point : points){
            point.draw(field, width, height, character);
        }
    }
    virtual void drawPointsBetween(char** field, int const width, int const height, char character) const{
        for(int i = 1; i < points.size(); i++){
            (points[i-1]*0.75f + points[i]*0.25f).draw(field, width, height, character);
            (points[i-1]*0.50f + points[i]*0.50f).draw(field, width, height, character);
            (points[i-1]*0.25f + points[i]*0.75f).draw(field, width, height, character);
        }
    }
};

std::ostream& operator << (std::ostream& os, Polyline& broken) {
    return broken.print(os);
}

class PolylineClosed: public Polyline{
public:
    explicit PolylineClosed(vector<Point> const& _points): Polyline(_points){}

    float length() const override{
        if(points.size() > 2){
            return Polyline::length() + points[0].distance(points[points.size()-1]);
        }
        return Polyline::length();
    }

    void draw(char** field, int const width, int const height) const override{
        cout << "Draw Polyline Closed" << endl;
        drawPoints(field, width, height, 'C');
        drawPointsBetween(field, width, height, 'C');
    }

protected:
    PolylineClosed() = default;
    void drawPointsBetween(char** field, int const width, int const height, char character) const override{
        Polyline::drawPointsBetween(field, width, height, character);
        (points[0]*0.75f + points[points.size()-1]*0.25f).draw(field, width, height, character);
        (points[0]*0.50f + points[points.size()-1]*0.50f).draw(field, width, height, character);
        (points[0]*0.25f + points[points.size()-1]*0.75f).draw(field, width, height, character);
    }
};

class Polygon: public PolylineClosed{
public:
    explicit Polygon(vector<Point> const& _points): PolylineClosed(_points){}
    float square(){
        size_t length = points.size();
        if(length<3)
            return 0;
        float square = 0;
        for(int i = 1; i < length; i++){
            square += points[i-1].x * points[i].y - points[i].x*points[i-1].y;
        }
        square += points[length-1].x*points[0].y - points[0].x*points[length-1].y;
        return abs(square)/2;
    }
    float perimeter(){
        return length();
    }

    void draw(char** field, int const width, int const height) const override{
        cout << "Draw Polygon" << endl;
        drawPoints(field, width, height, 'P');
        drawPointsBetween(field, width, height, 'P');
    }
protected:
    Polygon() = default;
private:
    float length() const{
        return PolylineClosed::length();
    };
};

class Triangle: public Polygon{
public:
    explicit Triangle(Point const& p1, Point const& p2, Point const& p3){
        points.push_back(p1);
        points.push_back(p2);
        points.push_back(p3);
    }
    void draw(char** field, int const width, int const height) const override{
        cout << "Draw Triangle" << endl;
        drawPoints(field, width, height, 'T');
        drawPointsBetween(field, width, height, 'T');
    }
};

class Trapeze: public Polygon{
public:
    explicit Trapeze(Point const& base1A, Point const& base1B, Point const& base2A, float const base2Ratio){
        points.push_back(base1B);
        points.push_back(base1A);
        points.push_back(base2A);
        Point base2B = base2A + (base1B-base1A)*base2Ratio;
        points.push_back(base2B);
    }
    void draw(char** field, int const width, int const height) const override{
        cout << "Draw Trapeze" << endl;
        drawPoints(field, width, height, 'r');
        drawPointsBetween(field, width, height, 'r');
    }
};

class PrimeShape: public Polygon{
public:
    explicit PrimeShape(vector<Point> const& _points) : Polygon(_points){
        int length = _points.size();
        if(length == 0){
            throw invalid_argument("0 points passed into PrimeShape Constructor");
        }
        float edgeLength = _points[0].distance(_points[length-1]);
        for(int i = 1; i < length; i++){
            float distance = points[i-1].distance(points[i]);
            if(abs(edgeLength-distance) > 0.001){
                throw invalid_argument("Points in PrimeShape Constructor do not describe PrimeShape");
            }
        }
    }

    void draw(char** field, int const width, int const height) const override{
        cout << "Draw Prime Shape" << endl;
        drawPoints(field, width, height, 's');
        drawPointsBetween(field, width, height, 's');
    }
};

void draw(vector<Polyline*> const& figures, int width, int height){
    char** field = (char**)malloc(sizeof(char*) * height);
    for(int j = 0; j < height; j++){
        field[j] = (char*)malloc(width+1);
        for(int i = 0; i < width; i++){
            field[j][i] = ' ';
        }
        field[j][width] = 0;
    }
    for(Polyline* const& figure : figures){
        figure->draw(field, width, height);
    }
    for(int j = 0; j < height; j++){
        cout << field[j] << endl;
        free(field[j]);
    }
    free(field);
}

int main() {
    cout << "POINT" << endl;

    Point pdef = Point{};

    Point p = Point(1,1);
    cout << "Point: " << p << endl;

    Point p2 = p;
    p2.x = 12;
    cout << "Point: " << p << "   Point 2: " << p2 << endl;



    cout << endl << "POLYLINE" << endl;

    vector<Point> points;
    points.push_back(Point(1, 1));
    points.push_back(Point(1, 2));
    points.push_back(Point(2, 1));
    /*points.reserve(10);
    for(int i = 0; i < 10; i++){
        points.emplace_back(1,i);
    }*/
    Polyline polyline = Polyline(points);
    cout << "Polyline 1: " <<  polyline << ". Its perimeter: " << polyline.length() << endl;
    Polyline polyline2 = Polyline(polyline);
    cout << "Polyline 2: " <<  polyline2 << ". Its perimeter: " << polyline2.length() << endl;
    Polyline polyline3 = polyline;
    cout << "Polyline 3: " <<  polyline3 << ". Its perimeter: " << polyline3.length() << endl;

    cout << endl << "CLOSED POLYLINE" << endl;

    PolylineClosed closedPolyline = PolylineClosed(points);
    cout << "Closed polyline 1: " <<  closedPolyline << ". Its perimeter: " << closedPolyline.length() << endl;
    PolylineClosed closedPolyline2 = PolylineClosed(closedPolyline);
    cout << "Closed polyline 2: " <<  closedPolyline2 << ". Its perimeter: " << closedPolyline2.length() << endl;
    PolylineClosed closedPolyline3 = closedPolyline;
    cout << "Closed polyline 3: " <<  closedPolyline3 << ". Its perimeter: " << closedPolyline3.length() << endl;

    cout << endl << "POLYGON" << endl;

    Polygon polygon = Polygon(points);
    cout << "Polygon 1: " <<  polygon << ". Its perimeter: " << polygon.perimeter() << ". Its square: " << polygon.square() << endl;

    cout << endl << "TRIANGLE" << endl;

    Triangle triangle = Triangle(points[0], points[1], points[2]);
    cout << "Triangle 1: " <<  triangle << ". Its perimeter: " << triangle.perimeter() << ". Its square: " << triangle.square() << endl;

    cout << endl << "TRAPEZE" << endl;

    Trapeze trapeze = Trapeze(points[0], points[1], points[2], 2);
    cout << "Trapeze 1: " <<  trapeze << ". Its perimeter: " << trapeze.perimeter() << ". Its square: " << trapeze.square() << endl;

    cout << endl << "PRIME SHAPE" << endl;

    try{
        PrimeShape primeShape = PrimeShape(points);
        cout << "PrimeShape 1: " <<  primeShape << ". Its perimeter: " << primeShape.perimeter() << ". Its square: " << primeShape.square() << endl;
    } catch (invalid_argument e) {
        cout << e.what() << endl;
    }
    points.pop_back();
    points.push_back(Point(2,2));
    points.push_back(Point(2,1));
    PrimeShape primeShape2 = PrimeShape(points);
    cout << "PrimeShape 2: " <<  primeShape2 << ". Its perimeter: " << primeShape2.perimeter() << ". Its square: " << primeShape2.square() << endl;
    PrimeShape primeShape3 = PrimeShape(primeShape2);
    cout << "PrimeShape 3: " <<  primeShape3 << ". Its perimeter: " << primeShape3.perimeter() << ". Its square: " << primeShape3.square() << endl;

    cout << endl << "Polymorphism" << endl;


    vector<Polyline*> figures;

    figures.push_back(new Polyline(polyline.scale(4, 4)));

    closedPolyline.move(3.1f, 0).scale(4,4);
    figures.push_back(new PolylineClosed(closedPolyline));

    polygon.move(0.1f, 3.1f).scale(4, 4);
    figures.push_back(new Polygon(polygon));

    triangle.move(3.1f, 3.1f).scale(4, 4);
    figures.push_back(new Triangle(triangle));

    primeShape2.move(6.1f, 0).scale(4, 4);
    figures.push_back(new PrimeShape(primeShape2));

    draw(figures, 60, 21);

    for(Polyline* const& figure : figures){
        delete figure;
    }

    return 0;
}


