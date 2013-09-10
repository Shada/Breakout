
Ball::Ball()
{

}

Ball::~Ball()
{

}

Ball::update(double _dt)
{
	/*
	Do collision check here
	*/
	possition += direction*speed;
}