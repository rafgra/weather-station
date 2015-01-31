<?php if ( !defined( 'BASEPATH' ) ) exit( 'No direct script access allowed' );
class Api extends CI_Controller {

  public function last() {
    header( 'Content-Type: application/json' );
    $iso = $this->input->get( "iso" );
    $lastHour = $this->input->get( "hours" );
    $formatDate = array( 'enabled' => !empty( $iso ) );
    if ( $iso == '2' ) {
      $formatDate['pattern'] = '%d/%m/%Y %H:%i:%s';
    }

    $data = $this->records->last( !empty( $lastHour ), $formatDate );
    echo json_encode( $data );
  }

  public function hours() {
    header( 'Content-Type: application/json' );
    $iso = $this->input->get( "iso" );
    $limit = $this->input->get( "limit" );
    $order = $this->input->get( "order" );
    $limit =  ( !empty( $limit ) ) ? ' -'. $limit.' seconds':null;
    $order = ( !empty( $order ) )?$order:'asc';
    $formatDate = array( 'enabled' => !empty( $iso ) );
    if ( $iso == '2' ) {
      $formatDate['pattern'] = '%d/%m/%Y %H:%i:%s';
    }

    $data = $this->records->get( true, $formatDate , $limit , $order );
    echo json_encode( $data );
  }

  public function all() {
    header( 'Content-Type: application/json' );
    $iso = $this->input->get( "iso" );
    $limit = $this->input->get( "limit" );
    $order = $this->input->get( "order" );
    $limit =  ( !empty( $limit ) ) ? ' -'. $limit.' seconds':null;
    $order = ( !empty( $order ) )?$order:'asc';
    $formatDate = array( 'enabled' => !empty( $iso ) );
    if ( $iso == '2' ) {
      $formatDate['pattern'] = '%d/%m/%Y %H:%i:%s';
    }

    $data = $this->records->get( false, $formatDate , $limit , $order );
    echo json_encode( $data );
  }

}
