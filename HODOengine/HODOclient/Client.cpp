#include "Client.h"
#include "TestScene.h"

Client::Client()
{

}

Client::~Client()
{

}

void Client::Initialize()
{
	TestScene test;
	test.Start();
}