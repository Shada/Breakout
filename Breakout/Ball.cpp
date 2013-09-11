
Logic::Ball::Ball()
{

}

Logic::Ball::~Ball()
{

}

Logic::Ball::update(double _dt)
{
	/*
	Do collision check here
	*/
	possition += direction*speed;
}