#include "ofApp.h"
#include "ofxClipper.h"
//--------------------------------------------------------------
void ofApp::setup(){
  i=0;
  triggerUpdate=true;
  frame = 0;
  gui.setup();

  gui.add(outline.setup("outline", false));
  gui.add(letters.setup("letters", false));
  gui.add(noiseOut.setup("noiseOut", false));
gui.add(fontScale.setup("fontScale", false));



  gui.add(wheelSpacing.setup("wheel spacing", 2, 1, 1000));
  gui.add(chairSpacing.setup("person spacing", 2, 1, 1000));
  gui.add(strokes.setup("strokes", 10, 1, 500));
  gui.add(lineSpacing.setup("lineSpacing", 2, -100, 100));
  gui.add(smoothed.setup("smoothed", 2, 1, 10));
  gui.add(letterSpacing.setup("letterSpacing", 10.0, 0.1, 50.0));
  gui.add(fontFactor.setup("fontFactor", 0.0, 0.0, 10.0));
  gui.add(fudge.setup("fudge", 0.0, 0.0, 10.0));
  gui.add(smoothShape.setup("smoothShape", 0.0, 0.0, 1.0));
  gui.add(startSmoothing.setup("startSmoothing", 10, 1, 100));
  gui.add(noiseRatio.setup("noiseRatio", 1.0, 0.0, 100.0));
  gui.add(noiseMin.setup("noiseMin", -1.0, -100.0, 0.0));
  gui.add(noiseMax.setup("noiseMax", 1.0, 0.0, 100.0));
  gui.add(fontStart.setup("fontStart", 1, 1, 100.0));
  gui.add(fontInc.setup("fontInc", 1, 1, 100.0));

  gui.add(chanceGitch.setup("chanceGitch", 1, 1, 1000.0));



  gui.add(noiseStart.setup("noiseStart", 1, 1, 100));
  gui.add(octaves.setup("octaves", 1, 0, 100));
  gui.add(seed.setup("seed", 1, 1, 100));
  gui.add(freq.setup("freq", 1.0, 0.0, 500.0));
  gui.add(amp.setup("amp", 0.5, 0.0, 100.0));


  gui.loadFromFile("settings.xml");
  svg.load("Wheelchair_withskeleton.svg");

  svgLayer.load("1layer.svg");

  int pathCount = svgLayer.getNumPath();

  for(int i=0; i< pathCount; i++){
    ofPath layerPath = svgLayer.getPathAt(i);
    layerPath.setPolyWindingMode(OF_POLY_WINDING_ODD);
    svgPolyLayers.push_back(layerPath);
  }

  ofPath personPath = svg.getPathAt(0);
  ofPath chairPath = svg.getPathAt(1);

  ofPath personSkeletonPath = svg.getPathAt(2);
  ofPath chairSkeletonPath  = svg.getPathAt(3);

  personPath.setPolyWindingMode(OF_POLY_WINDING_ODD);
  chairPath.setPolyWindingMode(OF_POLY_WINDING_ODD);
  personSkeletonPath.setPolyWindingMode(OF_POLY_WINDING_ODD);
  chairSkeletonPath.setPolyWindingMode(OF_POLY_WINDING_ODD);

  person = personPath.getOutline()[0];
  chair = chairPath.getOutline()[0];
  personSkeleton = personSkeletonPath.getOutline()[0];
  chairSkeleton = chairSkeletonPath.getOutline()[0];


  sentences.push_back("A model of enforcement that relies on litigation by private individuals. ");
  sentences.push_back("Placed at a disadvantage by the situation or actions that they are seeking to challenge. ");
  //  sentences.push_back("There is a real need for a proactive, concerted, effort on the part of mainstream systems and structures. ");
  //  sentences.push_back("Take seriously the challenge of creating an inclusive environment. ");
  //  sentences.push_back("Equality Act 2010 is not having the kind of impact that it was expected to have. ");
  //  sentences.push_back("Twenty years since the 1995 Disability Discrimination Act first set out the standards expected of them. ");
  //sentences.push_back("Deliver the change that disabled people, and society, need if our laws on disability discrimination are to have the impact that was intended. ");
  //  sentences.push_back("Our current, all-too-often inaccessible, built environment. ");


  for(int i=0; i < 50; i+= fontInc){
    ofTrueTypeFont text;
    //    text.load("/Users/josephwilk/Library/Fonts/Transport Medium.ttf", 7+i*2, true, true, true);
    //    text.load("/Users/josephwilk/Library/Fonts/Transport Medium.ttf", 7+i*2, true, true, true);
    text.load("/Users/josephwilk/Library/Fonts/DroidSansMono.ttf",  fontStart, true, true, true);
    fonts.push_back(text);
  }

  //  personSkeleton.addVertices(personSkeleton2.getVertices());

  //chairSkeleton.close();
  //
    //chair.scale(0.5,0.5);
  //  person.scale(0.5,0.5);
  //  chairSkeleton.scale(0.5,0.5);
  //  personSkeleton.scale(0.5,0.5);

  //int spacing = 50;
  chair = chair.getResampledBySpacing(wheelSpacing);
  person = person.getResampledBySpacing(chairSpacing);
  personSkeleton = personSkeleton.getResampledBySpacing(wheelSpacing);
  chairSkeleton = chairSkeleton.getResampledBySpacing(chairSpacing);
}


ofPolyline ofApp::burbPolyline(ofPolyline p0, int strokeCount, int dir){
  ofPolyline p;
  vector<glm::vec3> p0Verts = p0.getVertices();

  ofRectangle rect1 = fonts[strokeCount % fonts.size()].getStringBoundingBox("f", 0,0);
  ofRectangle rect2 = fonts[strokeCount % fonts.size()].getStringBoundingBox("o", 0,0);
  ofRectangle rect3 = fonts[strokeCount % fonts.size()].getStringBoundingBox("g", 0,0);
  int i=0;
  for(auto& v : p0Verts){
    if(strokeCount==0){
      p.addVertex(v);
    }
    else{
      float fontLineSpacing = (rect1.height - rect2.height) + (rect3.height - rect2.height) + rect2.height  + lineSpacing;
      if(!fontScale){
        fontLineSpacing = lineSpacing;
      }
      if((rand() % chanceGitch) == 0){
        fontLineSpacing += ofRandom(noiseRatio);
      }
      glm::vec3 v2 = p0Verts[(i+1) % p0Verts.size()];
      ofVec2f perpendicular = ofVec2f( (v - v2) ).getPerpendicular() * (dir * fontLineSpacing);
      glm::vec3 extendedVert = glm::vec3(v) + glm::vec2(perpendicular);
      if(!p0.inside(extendedVert)){
        p.addVertex(extendedVert);
      }
    }
    i++;
  }
  p.addVertex(p.getVertices()[0]);
  p.close();
  return p;
}


void ofApp::drawLetters(ofPolyline line, int strokeCount){
  ofRectangle rect = fonts[strokeCount % fonts.size()].getStringBoundingBox("a", 0,0);
  float fontLetterSpacing = rect.width + letterSpacing ;
  //fontLetterSpacing = letterSpacing;

  line = line.getResampledBySpacing(fontLetterSpacing);
  vector<glm::vec3> polyVerts = line.getVertices();
  std::reverse(polyVerts.begin(), polyVerts.end());

  msa::Perlin perlin = msa::Perlin(octaves, freq,amp,seed);
  //perlin.setup();


  int letter_count=0;
  int sentence_index=0;
  int i=0;
  for(auto& v : polyVerts){
    glm::vec3 v2 = polyVerts[(i+1) % polyVerts.size()];
    ofPushMatrix();
    glm::vec3 middle= ofVec3f(v).middle(v2);
    //    ofVec2f perpendicular = ofVec2f( (v - v2) ).getPerpendicular() * lineSpacing;
    //    ofVec2f newPoint = ofVec2f(middle)+perpendicular;
    ofTranslate((ofVec2f(middle)).x, (ofVec2f(middle)).y);

    float angle1 = atan2(middle.y - middle.y, middle.x - (middle.x+1000));
    float angle2 = atan2(middle.y - v.y, v.x - middle.x);
    float calculatedAngle = (angle1 - angle2);

    ofRotateRad(calculatedAngle);

    if(letters){
      if((!person.inside(middle) && (!chair.inside(middle))) || strokeCount == 0){
        // && !strokedPolys[strokeCount].inside(middle) ) || strokeCount==0){
        std::string candidate;
        candidate = sentences[(strokeCount+sentence_index) % sentences.size()];
        //        candidate = sentences[(strokeCount+sentence_index) % sentences.size()];
        if(strokeCount == (strokes-1) || strokeCount == (strokes)){
          //letter_count=0;
          //candidate = "Deliver the change that disabled people, and society, need if our laws on disability discrimination are to have the impact that was intended. ";
        }
        if(letter_count >= candidate.size()){
          //sentence_index++;
          letter_count=0;
        }

        std::string t(1, candidate.at(letter_count % candidate.size()));
        //        fonts[strokeCount].drawString(t, 0, 0);



        if(noiseOut || (octaves+(strokeCount-noiseStart)*noiseRatio < 1)){
          fonts[strokeCount].drawString(t, 0, 0);
        }
        else{


          bool vflip = false; // OF flips y coordinate in the default perspective,
          bool filled = false; // or false for contours

          //vector < ofPath > paths = fonts[2].getStringAsPoints(t, vflip, filled);
          vector < ofPath > paths = fonts[strokeCount].getStringAsPoints(t, vflip, filled);

          perlin.setup(octaves+(strokeCount-noiseStart)*noiseRatio,freq,amp+(strokeCount-noiseStart)*noiseRatio,seed);

          for (int iz = 0; iz < paths.size(); iz++){
            vector <ofPolyline> polylines = paths[iz].getOutline(); //Turns out too thick. Need to change the path, not OUTLINE

            for (auto& poly : polylines) {
              poly = poly.getResampledBySpacing(0.5);
              vector<glm::vec3> vs = poly.getVertices();
              ofPolyline p;
              int idx=0;
              for (auto& vert : vs) {
                float noize = perlin.get(vert.x, vert.y);
                vert.x += noize; //ofRandom(noiseMin*strokeCount, noiseMax*strokeCount);
                vert.y += noize; //ofRandom(noiseMin*strokeCount, noiseMax*strokeCount);
                vs[idx] = vert;
                idx++;
                ofDrawCircle(vert.x, vert.y, 0.25);
              }

              //            for(int noise=0; noise < (strokeCount-noiseStart)*noiseRatio; noise++){
              //              int idx = rand() % vs.size() ;
              //              glm::vec3 point = vs[idx];
              //              if(strokeCount > noiseStart){
              //                float noize = perlin.get(point.x, point.y);
              //                point.x += noize; //ofRandom(noiseMin*strokeCount, noiseMax*strokeCount);
              //                point.y += noize;//ofRandom(noiseMin*strokeCount, noiseMax*strokeCount);
              //                //point.z += ofRandom(noiseMin, noiseMax);
              //              }
              //              vs[idx] = point;
              //            }
              //p = ofPolyline(vs);
              //p.close();
              //p.draw();
            }
          }
        }

      }
    }
    //fonts[0].drawString(std::string(eqAct.), 0, 0)
    ofPopMatrix();
    i++;
    letter_count++;
  }
}


ofPolyline ofApp::removeIntersects(ofPolyline p){
  ofPolyline cleanP;
  vector<glm::vec3> currentLines = p.getVertices();
  vector<ofPolyline> polyCollection;

  for(int idx=0; idx < currentLines.size(); idx++){
    bool inIntersect = false;
    glm::vec3 p1 = currentLines[idx];
    glm::vec3 p2 = currentLines[(idx+1) % currentLines.size()];
    glm::vec3 iv = glm::vec3(0.0,0.0,0.0);

    for(int lineIdx=0; lineIdx < currentLines.size(); lineIdx++){
      glm::vec3 lineStart = currentLines[lineIdx];
      glm::vec3 lineEnd = currentLines[(lineIdx+1) % currentLines.size()];

      if(lineStart != p1 && lineStart != p2 && lineEnd != p1 && lineEnd != p2){
        if(ofLineSegmentIntersection(lineStart, lineEnd,
                                     p1, p2, iv)){
          //ofDrawLine(lineStart, lineEnd);
          //ofDrawLine(currentLines[currentLines.size()-1], extendedVert);
          // ofDrawCircle(iv.x, iv.y, 5);
          inIntersect = true;
          break;
        }
      }
    }

    if(inIntersect){
      cleanP.addVertex(iv);
      cleanP.close();
      polyCollection.push_back(cleanP);

      cleanP.clear();
      cleanP.addVertex(p1);
    }
    else{
      if(idx == currentLines.size()-1){
        //        cleanP.addVertex(p1);
        //        cleanP.close();
        //        polyCollection.push_back(cleanP);
      }else{
        cleanP.addVertex(p1);
      }
    }
  }

  ofPolyline finalPoly;
  glm::vec3 lastVert = glm::vec3(0,0,0);
  for(int i=0; i < polyCollection.size(); i++){
    ofPolyline p1 = polyCollection[i];
    glm::vec3 firstp1 = p1.getVertices()[0];
    glm::vec3 lastp1 = p1.getVertices()[p1.getVertices().size()-1];

    if((lastVert == glm::vec3(0,0,0)) || lastp1 != lastVert){
      //  p1.draw();
      int z=0;
      for(auto& v : p1.getVertices()){
        //  if(z != p1.getVertices().size()-1){
        finalPoly.addVertex(v);
        //   }
        z++;
      }
    }
    lastVert = p1.getVertices()[p1.getVertices().size()-1]; //intersect point
  }

  if(finalPoly.size() > 0){
    finalPoly.addVertex(finalPoly.getVertices()[0]);
    finalPoly.close();
  }
  else{
    finalPoly = p;
  }
  return finalPoly;
}

//--------------------------------------------------------------
void ofApp::update(){
  if(triggerUpdate){
    
    allLines.clear();

    vector<ofPolyline> strokedPolys;
    ofPolyline previousPolyPerson = person;
    ofPolyline previousPolyChair = chair;
//
//    previousPolyChair = previousPolyChair.getSmoothed(smoothed+1, smoothShape);
//    previousPolyPerson = previousPolyPerson.getResampledBySpacing(chairSpacing);
//    previousPolyChair =  previousPolyChair.getResampledBySpacing(wheelSpacing);




    for(int strokeCount=0; strokeCount < strokes; strokeCount++){
      ofPolyline polyPerson = person;
      ofPolyline polyChair = chair;

      polyPerson = burbPolyline(previousPolyPerson, strokeCount, -1);
      polyChair  = burbPolyline(previousPolyChair,  strokeCount, 1);

      //ofPolyline finalPoly = removeIntersects(p);

      //i++;

      //}

//      ofPolyline finalPoly = polyPerson;
//      if(strokeCount > 1){
//        polyPerson.clear();
//
//        if(strokeCount > startSmoothing){
//          finalPoly = finalPoly.getSmoothed(smoothed+1, smoothShape);
//        }
//        else{
//          finalPoly = finalPoly.getSmoothed(smoothed, smoothShape);
//        }
//        polyPerson=finalPoly;
//      }
//      else{
//        if(strokeCount == 1){
//          polyPerson = polyPerson.getSmoothed(smoothed, smoothShape);
//        }
//      }

      //if(strokeCount < 15 ){
    //  polyChair = polyChair.getSmoothed(smoothed+1, smoothShape);
      polyPerson = polyPerson.getResampledBySpacing(chairSpacing);
      polyChair =  polyChair.getResampledBySpacing(wheelSpacing);



      bool clip=true;
      vector<ofPolyline> intersections;
      if(clip){
        ofx::Clipper clipper;
        clipper.addPolyline(polyPerson, ClipperLib::ptSubject);
        clipper.addPolyline(polyChair, ClipperLib::ptClip);
        intersections = clipper.getClipped(ClipperLib::ClipType::ctUnion);
      }
      else{
        intersections.push_back(polyPerson);
        intersections.push_back(polyChair);
      }
      allLines.push_back(intersections);

      previousPolyPerson = polyPerson;
      previousPolyChair = polyChair;
      strokedPolys.push_back(previousPolyPerson);
    }

    triggerUpdate=false;
  }
}

//--------------------------------------------------------------
void ofApp::draw(){
  ofPtr<ofCairoRenderer> cairo(new ofCairoRenderer);
  ofPtr<ofCairoRenderer> cairo2(new ofCairoRenderer);
  ofBackground(255, 255, 255);
  ofPushMatrix();
  ofTranslate(ofGetWindowWidth()/4.0, ofGetWindowHeight()/5.0);


  if(toCVG){
    cairo->setup("lines"+ ofToString(ofGetTimestampString())+".svg", ofCairoRenderer::SVG, false, false, ofRectangle(0, 0, 4000, 4000));
    //cairo2->setup("lines2"+ ofToString(ofGetTimestampString())+".svg", ofCairoRenderer::SVG, false, false, ofRectangle(0, 0, 0, 0));
    auto d = ofGetCurrentRenderer();
    ofSetCurrentRenderer(cairo);
    //    ofStyle style = ofGetCurrentRenderer()->getStyle();
    //    ofGetCurrentRenderer()->setupGraphicDefaults();
    //    ofGetCurrentRenderer()->setStyle(style);

    ofBackground(255, 255, 255);

    ofTranslate(2000, 2000);
    ofPushMatrix();
    //ofTranslate(500,200);
    ofSetLineWidth(1.0);
    ofSetColor(0, 0, 0, 255);


    int strokeCount=0;
    for(auto& lines : allLines){
      for (auto& line: lines){
        if(outline){
          if(strokeCount !=0){
          line.draw();
          }
        }
        drawLetters(line, strokeCount);
      }
      strokeCount++;
    }


    ofPopMatrix();
    ofSetCurrentRenderer(d);
    toCVG=false;
  }

  ofSetColor(0, 0, 0, 255);
  ofNoFill();

  int strokeCount=0;
  for(auto& lines : allLines){
    for (auto& line: lines){
      if(outline){
        if(strokeCount !=0){
          line.draw();
        }

      }
      drawLetters(line, strokeCount);
    }
    strokeCount++;
  }

  ofPopMatrix();
  gui.draw();
  frame+=1;
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
  if(key == 't'){
    triggerUpdate=true;
  }
  if(key == 'r'){
    toCVG = true;
  }
  if(key == 'u'){
    //int spacing = 50;
    ofPath personPath = svg.getPathAt(0);
    ofPath chairPath = svg.getPathAt(1);

    ofPath personSkeletonPath = svg.getPathAt(2);
    ofPath chairSkeletonPath  = svg.getPathAt(3);

    personPath.setPolyWindingMode(OF_POLY_WINDING_ODD);
    chairPath.setPolyWindingMode(OF_POLY_WINDING_ODD);
    personSkeletonPath.setPolyWindingMode(OF_POLY_WINDING_ODD);
    chairSkeletonPath.setPolyWindingMode(OF_POLY_WINDING_ODD);

    person = personPath.getOutline()[0];
    chair = chairPath.getOutline()[0];
    personSkeleton = personSkeletonPath.getOutline()[0];
    chairSkeleton = chairSkeletonPath.getOutline()[0];

    chair = chair.getResampledBySpacing(wheelSpacing);
    person = person.getResampledBySpacing(chairSpacing);
    personSkeleton = personSkeleton.getResampledBySpacing(chairSpacing);
    chairSkeleton = chairSkeleton.getResampledBySpacing(wheelSpacing);

    polys.clear();
    fonts.clear();
    for(int i=0; i < 50; i+= fontInc){
      ofTrueTypeFont text;
      int newFontSize = fontStart;
      if(fontScale){
        newFontSize += i;
      }
      text.load("/Users/josephwilk/Library/Fonts/DroidSansMono.ttf",  newFontSize, true, true, true);
      fonts.push_back(text);
    }
    frame=0;
  }
  if(key == 's'){
    gui.saveToFile("settings.xml");
  }
  else if(key == 'l'){
    gui.loadFromFile("settings.xml");
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
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

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
