#include "NBTreeClassifierTree.h"
#include "ModelSelection.h"
#include "Instances.hpp"



                NBTreeClassifierTree::NBTreeClassifierTree( ModelSelection *toSelectLocModel ) : ClassifierTree( toSelectLocModel ) {
                }


                void NBTreeClassifierTree::buildClassifier( Instances *data ) {
                  ClassifierTree::buildClassifier( data );
                  cleanup( new Instances( data, 0 ) );
                  assignIDs( -1 );
                }

                ClassifierTree *NBTreeClassifierTree::getNewTree( Instances *data ) {

                  ClassifierTree *newTree = new NBTreeClassifierTree( mtoSelectModel );
                  newTree->buildTree( data, false );

                  return newTree;
                }

                ClassifierTree *NBTreeClassifierTree::getNewTree( Instances *train, Instances *test ) {

                  ClassifierTree *newTree = new NBTreeClassifierTree( mtoSelectModel );
                  newTree->buildTree( train, test, false );

                  return newTree;
                }

                std::string NBTreeClassifierTree::printLeafModels() {
                  std::string text = "";

                  if( misLeaf ) {
                    text.append( std::string( "\nLeaf number: " ) + mid + std::string( " " ) );
//JAVA TO C++ CONVERTER TODO TASK: There is no native C++ equivalent to 'toString':
                    text.append( mlocalModel->toString() );
                    text.append( "\n" );
                  } else {
                    for( ClassifierTree *mson : msons ) {
                      text.append( ( static_cast<NBTreeClassifierTree*>( mson ) )->printLeafModels() );
                    }
                  }
//JAVA TO C++ CONVERTER TODO TASK: There is no native C++ equivalent to 'toString':
                  return text;
                }

                std::string NBTreeClassifierTree::toString() {

                  try {
                    std::string text = "";

                    if( misLeaf ) {
                      text.append( ": NB" );
                      text.append( mid );
                    } else {
                      dumpTreeNB( 0, text );
                    }

                    text.append( std::string( "\n" ) + printLeafModels() );
                    text.append( std::string( "\n\nNumber of Leaves  : \t" ) + numLeaves() + std::string("\n") );
                    text.append( std::string( "\nSize of the tree : \t" ) + numNodes() + std::string("\n") );

                    return text;
                  } catch( std::exception &e ) {
                    std::cout << e.what();
                    return "Can't print nb tree.";
                  }
                }

                void NBTreeClassifierTree::dumpTreeNB( int depth, StringBuffer *text ) {

                  int i, j;

                  for( i = 0; i < msons::length; i++ ) {
                    text.append( "\n" );
                    ;
                    for( j = 0; j < depth; j++ ) {
                      text.append( "|   " );
                    }
                    text.append( mlocalModel::leftSide( mtrain ) );
                    text.append( mlocalModel::rightSide( i, mtrain ) );
                    if( msons[i].misLeaf ) {
                      text.append( ": NB " );
                      text.append( msons[i].mid );
                    } else {
                      ( static_cast<NBTreeClassifierTree*>( msons[i] ) )->dumpTreeNB( depth + 1, text );
                    }
                  }
                }

                std::string NBTreeClassifierTree::graph() throw(std::exception) {

                  std::string text = "";

                  text.append( "digraph J48Tree {\n" );
                  if( misLeaf ) {
                    text.append( std::string( "N" ) + mid + std::string( " [label=\"" ) + std::string( "NB mode" ) + std::string( "\" " ) + std::string( "shape=box style=filled " ) );
                    if( mtrain != nullptr && mtrain::numInstances() > 0 ) {
                      text.append( std::string( "data =\n" ) + mtrain + std::string( "\n" ) );
                      text.append( ",\n" );

                    }
                    text.append( "]\n" );
                  } else {
                    text.append( std::string( "N" ) + mid + std::string( " [label=\"" ) + Utils::backQuoteChars( mlocalModel::leftSide( mtrain ) ) + std::string( "\" " ) );
                    if( mtrain != nullptr && mtrain::numInstances() > 0 ) {
                      text.append( std::string( "data =\n" ) + mtrain + std::string( "\n" ) );
                      text.append( ",\n" );
                    }
                    text.append( "]\n" );
                    graphTree( text );
                  }

                  return text + std::string("}\n");
                }

                void NBTreeClassifierTree::graphTree( StringBuffer *text ) {

                  for( int i = 0; i < msons::length; i++ ) {
                    text.append( std::string( "N" ) + mid + std::string( "->" ) + std::string( "N" ) + msons[i].mid + std::string( " [label=\"" ) + Utils::backQuoteChars( mlocalModel::rightSide( i, mtrain )->trim() ) + std::string("\"]\n") );
                    if( msons[i].misLeaf ) {
                      text.append( std::string( "N" ) + msons[i].mid + std::string( " [label=\"" ) + std::string( "NB Mode" ) + std::string( "\" " ) + std::string( "shape=box style=filled " ) );
                      if( mtrain != nullptr && mtrain::numInstances() > 0 ) {
                        text.append( std::string( "data =\n" ) + msons[i].mtrain + std::string( "\n" ) );
                        text.append( ",\n" );
                      }
                      text.append( "]\n" );
                    } else {
                      text.append( std::string( "N" ) + msons[i].mid + std::string( " [label=\"" ) + Utils::backQuoteChars( msons[i].mlocalModel::leftSide( mtrain ) ) + std::string( "\" " ) );
                      if( mtrain != nullptr && mtrain::numInstances() > 0 ) {
                        text.append( std::string( "data =\n" ) + msons[i].mtrain + std::string( "\n" ) );
                        text.append( ",\n" );
                      }
                      text.append( "]\n" );
                      ( static_cast<NBTreeClassifierTree*>( msons[i] ) )->graphTree( text );
                    }
                  }
                }
