#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    
    ofSetFrameRate(60);
    string host = Spacebrew::SPACEBREW_CLOUD; // "localhost";
    string name = "woof";
    string description = "It's amazing";
    
    spacebrew.addSubscribe("sliderX", "range");  // just typing "boolean" also works
    spacebrew.connect( host, name, description );
    Spacebrew::addListener(this, spacebrew);
    
    int sliderXValue;
    
    
    //load image
    image.loadImage("greenCandyFloss.png");
    
    
    myPlayer.loadMovie("example_small.gif");
    
    //image.resize(200, 200);
    
    //specify what primitive mode the mesh should use to connect the verticies
    //create mesh with points as the primitive
    //mesh.setMode(OF_PRIMITIVE_POINTS);
    
    //create mesh with lines as the primitive
    mesh.setMode(OF_PRIMITIVE_LINES);
    
    float intensityThreshold = 50.0;
    int w = image.getWidth();
    int h = image.getHeight();
    
    //loop through each pixel in the image using image width & height
    for (int x=0; x<w; x++){
        for(int y=0; y<h; y++){
            
            //create the color object at that pixel
            ofColor c = image.getColor(x, y);
            
            //check the intensity of the pixel's color
            float intensity = c.getLightness();
            
            //if the intensity exceeds the threshold, create a vertex at the location of the pixel
            //& color it with the pixel's color
            if (intensity >= intensityThreshold){
                
                //pushes brighter colors in the positive z direction
                //pushes whiter colors in the negative z direction
                float saturation = c.getSaturation();
                float z = ofMap(saturation, 0, 255, -300, 300);
                
                //the image is now 1/4 the size of the OF window, so multiply
                //the pixel location by 4 so that the mesh covers the OF window
                ofVec3f pos(4*x, 4*y, z);
                mesh.addVertex(pos);
                mesh.addColor(c);
            }
        }
    }
    
    //-------------adding lines-------------------------
    float connectionDistance = 40;
    int numVerts = mesh.getNumVertices();
    
    //finding all unique pairs of vertices
    for (int a=0; a<numVerts; a+=5){
        //get position of the ith vertex with .getVertex
        ofVec3f verta = mesh.getVertex(a);
        for(int b=a+1; b<numVerts; b++){
            ofVec3f vertb = mesh.getVertex(b);
            
            //distane() finds the distance btwn verticies
            float distance = verta.distance(vertb);
            if(distance <= connectionDistance){
                mesh.addIndex(a);
                mesh.addIndex(b);
            }
        }
    }
    
    
    //---------------orbiting-----------------------
    //getCentroid() finds the average location over all vertices in a mesh
    meshCentroid = mesh.getCentroid();
    
    for(int i=0; i<numVerts; i++){
        ofVec3f vert = mesh.getVertex(i);
        
        //find distance between vert and meshCentroid
        float distance = vert.distance(meshCentroid);
        
        //find angle between two vertices
        float angle = atan2(vert.y - meshCentroid.y, vert.x - meshCentroid.x);
        
        //create a vector of these distances
        distances.push_back(distance);
        
        //create a vector of these angles
        angles.push_back(angle);
    }
    
    //allow us to toggle orbit on & off
    orbiting = false;
    startOrbitTime = 0.0;
    
    //store a copy of the mesh, so that we can reload the original state
    meshCopy = mesh;
}

//--------------------------------------------------------------
void ofApp::update(){
    
    myPlayer.update(); // get all the new frames
    
    if(orbiting){
        int numVerts = mesh.getNumVertices();
        for(int i=0; i<numVerts; i++){
            ofVec3f vert = mesh.getVertex(i);
            float distance = distances[i];
            float angle = angles[i];
            float elapsedTime = ofGetElapsedTimef() - startOrbitTime;
            
            //things that are close to the center, move faster & vice versa
            float speed = ofMap(distance, 0, 200, 1, 0.25, true);
            
            //angular rotation of the vertex
            float rotatedAngle = elapsedTime*speed + angle;
            
            //shift everything back to screen coordinates by adding the x and y of the centroid
            vert.x = distance*cos(rotatedAngle) + meshCentroid.x;
            vert.y = distance*sin(rotatedAngle) + meshCentroid.y;
            
            mesh.setVertex(i, vert);
        }
    }
    
}

//--------------------------------------------------------------
void ofApp::draw(){
    
    
    
    //draw image at top left corner of screen
    //image.draw(0,0);
    
    ofColor centerColor = ofColor(85, 78, 68);
    ofColor edgeColor(0, 0, 0);
    ofBackgroundGradient(centerColor, edgeColor, OF_GRADIENT_CIRCULAR);
    
    myPlayer.draw(20,20); // draw at 20, 20 from the current transform matrix
	
    myPlayer.play();
    
    //print the number of vertices in the mesh
    //cout << mesh.getNumVertices() << endl;
    
    scale = ofMap(sliderXValue, -4, 4, 1, 25);
    
    
    //3D view
    //left click to drag to rotate
    //right click to drag to zoom
    easyCam.begin();
    ofPushMatrix();
    cout << scale << endl;
    ofScale(1,-1,scale);
    ofTranslate(-ofGetWidth()/2, -ofGetHeight()/2);
    
    //draw the mesh
    mesh.draw();
    
    ofPopMatrix();
    easyCam.end();
    
    
    
    
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    if(key == 'o'){
        
        //change boolean
        orbiting = !orbiting;
        startOrbitTime = ofGetElapsedTimef();
        //restores mesh to the original values
        mesh = meshCopy;
    }
    
}
//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}

//--------------------------------------------------------------
void ofApp::onMessage( Spacebrew::Message & m ){
    if ( m.name == "sliderX" ){
        //int sliderXValue = m.valueRange();
        sliderXValue = ofToInt(m.value) / 1024.0 * 500;
        cout << sliderXValue << endl;
        
    }
}

