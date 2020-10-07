#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {

	ofSetFrameRate(60);
	ofSetWindowTitle("openFrameworks");

	ofBackground(239);
	ofEnableDepthTest();

	this->frame.setMode(ofPrimitiveMode::OF_PRIMITIVE_LINES);
}

//--------------------------------------------------------------
void ofApp::update() {

	ofSeedRandom(39);

	this->face.clear();
	this->frame.clear();

	int radius = 30;
	for (int x = -300; x <= 300; x += radius * 2) {

		for (int y = -300; y <= 300; y += radius * 2) {

			auto noise_value = ofNoise(ofRandom(1000), ofGetFrameNum() * 0.005);
			auto deg_start = 0;

			if (noise_value < 0.2) { deg_start = 0; }
			else if (noise_value < 0.25) { deg_start = ofMap(noise_value, 0.2, 0.25, 0, 90); }
			else if (noise_value < 0.45) { deg_start = 90; }
			else if (noise_value < 0.50) { deg_start = ofMap(noise_value, 0.45, 0.50, 90, 180); }
			else if (noise_value < 0.70) { deg_start = 180; }
			else if (noise_value < 0.75) { deg_start = ofMap(noise_value, 0.7, 0.75, 180, 270); }
			else if (noise_value < 0.95) { deg_start = 270; }
			else if (noise_value < 1.0) { ofMap(noise_value, 0.95, 1.0, 270, 360); }

			auto height = 30;
			this->setRingToMesh(this->face, this->frame, glm::vec3(x, y, height * 0.5), radius - 2, 4, height, deg_start, deg_start + 270);
		}

	}
}

//--------------------------------------------------------------
void ofApp::draw() {

	this->cam.begin();
	ofRotateZ(ofGetFrameNum() * 0.5);

	ofSetColor(39);
	this->face.draw();

	ofSetColor(239);
	this->frame.drawWireframe();

	this->cam.end();
}

//--------------------------------------------------------------
void ofApp::setRingToMesh(ofMesh& face_target, ofMesh& frame_target, glm::vec3 location, float radius, float width, float height, int deg_start, int deg_end) {

	if (deg_start > deg_end) {

		auto tmp = deg_start - 1;
		deg_start = deg_end;
		deg_end = tmp;
	}

	int index = face_target.getNumVertices();
	int deg_span = 5;
	for (int deg = deg_start; deg <= deg_end; deg += deg_span) {

		auto face_index = face_target.getNumVertices();

		vector<glm::vec3> vertices;
		vertices.push_back(glm::vec3((radius + width * 0.5) * cos(deg * DEG_TO_RAD), (radius + width * 0.5) * sin(deg * DEG_TO_RAD), height * -0.5));
		vertices.push_back(glm::vec3((radius + width * 0.5) * cos((deg + deg_span) * DEG_TO_RAD), (radius + width * 0.5) * sin((deg + deg_span) * DEG_TO_RAD), height * -0.5));
		vertices.push_back(glm::vec3((radius + width * 0.5) * cos((deg + deg_span) * DEG_TO_RAD), (radius + width * 0.5) * sin((deg + deg_span) * DEG_TO_RAD), height * 0.5));
		vertices.push_back(glm::vec3((radius + width * 0.5) * cos(deg * DEG_TO_RAD), (radius + width * 0.5) * sin(deg * DEG_TO_RAD), height * 0.5));

		vertices.push_back(glm::vec3((radius - width * 0.5) * cos(deg * DEG_TO_RAD), (radius - width * 0.5) * sin(deg * DEG_TO_RAD), height * -0.5));
		vertices.push_back(glm::vec3((radius - width * 0.5) * cos((deg + deg_span) * DEG_TO_RAD), (radius - width * 0.5) * sin((deg + deg_span) * DEG_TO_RAD), height * -0.5));
		vertices.push_back(glm::vec3((radius - width * 0.5) * cos((deg + deg_span) * DEG_TO_RAD), (radius - width * 0.5) * sin((deg + deg_span) * DEG_TO_RAD), height * 0.5));
		vertices.push_back(glm::vec3((radius - width * 0.5) * cos(deg * DEG_TO_RAD), (radius - width * 0.5) * sin(deg * DEG_TO_RAD), height * 0.5));

		for (auto& vertex : vertices) {

			vertex += location;
		}

		face_target.addVertices(vertices);

		face_target.addIndex(face_index + 0); face_target.addIndex(face_index + 1); face_target.addIndex(face_index + 2);
		face_target.addIndex(face_index + 0); face_target.addIndex(face_index + 2); face_target.addIndex(face_index + 3);

		face_target.addIndex(face_index + 4); face_target.addIndex(face_index + 5); face_target.addIndex(face_index + 6);
		face_target.addIndex(face_index + 4); face_target.addIndex(face_index + 6); face_target.addIndex(face_index + 7);

		face_target.addIndex(face_index + 0); face_target.addIndex(face_index + 4); face_target.addIndex(face_index + 5);
		face_target.addIndex(face_index + 0); face_target.addIndex(face_index + 5); face_target.addIndex(face_index + 1);

		face_target.addIndex(face_index + 3); face_target.addIndex(face_index + 7); face_target.addIndex(face_index + 6);
		face_target.addIndex(face_index + 3); face_target.addIndex(face_index + 6); face_target.addIndex(face_index + 2);

		auto frame_index = frame_target.getNumVertices();

		frame_target.addVertices(vertices);

		frame_target.addIndex(frame_index + 0); frame_target.addIndex(frame_index + 1);
		frame_target.addIndex(frame_index + 2); frame_target.addIndex(frame_index + 3);
		frame_target.addIndex(frame_index + 4); frame_target.addIndex(frame_index + 5);
		frame_target.addIndex(frame_index + 6); frame_target.addIndex(frame_index + 7);
	}

	face_target.addIndex(index + 0); face_target.addIndex(index + 3); face_target.addIndex(index + 7);
	face_target.addIndex(index + 0); face_target.addIndex(index + 7); face_target.addIndex(index + 4);

	frame_target.addIndex(index + 0); frame_target.addIndex(index + 3);
	frame_target.addIndex(index + 0); frame_target.addIndex(index + 4);
	frame_target.addIndex(index + 7); frame_target.addIndex(index + 3);
	frame_target.addIndex(index + 7); frame_target.addIndex(index + 4);

	index = frame_target.getNumVertices() - 8;

	face_target.addIndex(index + 1); face_target.addIndex(index + 2); face_target.addIndex(index + 6);
	face_target.addIndex(index + 1); face_target.addIndex(index + 6); face_target.addIndex(index + 5);

	frame_target.addIndex(index + 1); frame_target.addIndex(index + 2);
	frame_target.addIndex(index + 1); frame_target.addIndex(index + 5);
	frame_target.addIndex(index + 6); frame_target.addIndex(index + 2);
	frame_target.addIndex(index + 6); frame_target.addIndex(index + 5);
}

//--------------------------------------------------------------
int main() {

	ofSetupOpenGL(720, 720, OF_WINDOW);
	ofRunApp(new ofApp());
}