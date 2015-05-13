/*
 *  cornellscene.cpp
 *  prTracer
 *
 *  Created by Rasmus Barringer on 2011-04-01.
 *  Copyright 2011 Lund University. All rights reserved.
 *
 */

#include "cornellscene.h"
#include "diffuse.h"
#include "sphere.h"
#include "mesh.h"
#include "emissive.h"

void buildCornellScene(Scene* scene)
{
	unsigned int index = 0;
	Diffuse* red = new Diffuse(Color(0.7f, 0.1f, 0.1f));
	Diffuse* blue = new Diffuse(Color(0.1f, 0.1f, 0.7f));
	Diffuse* white = new Diffuse(Color(0.7f, 0.7f, 0.7f));
	Diffuse* green = new Diffuse(Color(0.2f, 0.4f, 0.3f), 0.2f, 0.6f, 0.5);
	Diffuse* greenX = new Diffuse(Color(0.2f, 0.4f, 0.3f), 0.5f, 0.0f);
	Diffuse* whiteDiff = new Diffuse(Color(1.f, 1.f, 1.f));

	Mesh* ground = new Mesh("data/plane.obj", white);
	ground->setScale(150.0f);
	index = ground->setIndexes(index);

	Mesh* side1 = new Mesh("data/plane.obj", red); // left wall
	side1->setScale(150.0f);
	side1->setRotation(180.0f, 0.0f, 90.0f);
	side1->setTranslation(-60, 60, 0.0f);
	index = side1->setIndexes(index);

	Mesh* side2 = new Mesh("data/plane.obj", blue); // right wall
	side2->setScale(150.0f);
	side2->setRotation(0.0f, 0.0f, 90.0f);
	side2->setTranslation(60, 60, 0.0f);
	index = side2->setIndexes(index);

	Mesh* side3 = new Mesh("data/plane.obj", white); // far wall
	side3->setScale(150.0f);
	side3->setRotation(90.0f, 0.0f, 0.0f);
	side3->setTranslation(0.0f, 60, -60);
	index = side3->setIndexes(index);

	Mesh* roof = new Mesh("data/plane.obj", white);
	roof->setScale(150.0f);
	roof->setRotation(180.0f, 0.0f, 0.0f);
	roof->setTranslation(0.0f, 120, 0.0f);
	index = roof->setIndexes(index);

	Sphere* ball1 = new Sphere(16.0f, white);
	ball1->setTranslation(Vector3D(22.0f, 16.0f, 0.0f));
	index = ball1->setIndex(index);

	Sphere* ball2 = new Sphere(15.0f, green);
	ball2->setTranslation(Vector3D(-22.0f, 15.0f, 35.0f));
	index = ball2->setIndex(index);

	Sphere* ball3 = new Sphere(5.0f, blue);
	ball3->setTranslation(Vector3D(5.0f, 70.0f, 10.0f));
	index = ball3->setIndex(index);

	Sphere* ball4 = new Sphere(10.0f, greenX);
	ball4->setTranslation(Vector3D(-30.0f, 50.0f, 30.0f));
	index = ball4->setIndex(index);

	Sphere* ball5 = new Sphere(10.0f, red);
	ball5->setTranslation(Vector3D(-15.0f, 16.0f, 10.f));
	index = ball5->setIndex(index);

	PointLight* light = new PointLight(Point3D(0.0f, 100.0f, 60.0f), Color(1.0f, 1.0f, 1.0f), 12000.0f);

	scene->add(light);
	scene->add(ground); // index 0,1 down wall
	scene->add(side1); // index 2,3 left wall
	scene->add(side2); // index 4,5 right wall
	scene->add(side3); // index 6,7 far wall
	scene->add(roof); // index 8,9 up wall

	Sphere* ball6 = new Sphere(1.f, whiteDiff);
	ball6->setTranslation(Vector3D(-0.0f, 140.0f, 40.f));
	index = ball6->setIndex(index);

	scene->add(ball6); // index 11

	Mesh* elephant = new Mesh("data/f-16.obj", white);
	elephant->setScale(45.0f);
	elephant->setRotation(20.0f, 15.0f, 20.0f);
	elephant->setTranslation(0.0f, 50.0f, 30.f);
	index = elephant->setIndexes(index);

	scene->add(elephant);
}

void setupCornellCamera(Camera* camera)
{
	Vector3D up(0.0f, 1.0f, 0.0f);
	Point3D pos(0.0f, 60.0f, 180.0f);
	Point3D target(0.0f, 60.0f, 0.0f);
	camera->setLookAt(pos, target, up, 52.0f);
}
