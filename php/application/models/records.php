<?php
/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
*/
/**
 * Description of Records
 *
 * @author Oggy
 */
class Records extends CI_Model {

	private $endOfDayFormat = 'Y-m-d\T23:59:50P';
	private $startOfDayFormat = 'Y-m-d\T00:00:00P';

	function __construct() {
		parent::__construct();
		$this->db->select( 'temperature, pressure, humidity, illuminance, dewpoint' );
	}

	private function _formatDate( $pattern = null ) {
		if ( $pattern  == null ) {
			$this->db->select_date( 'date' );
		}else {
			$this->db->select_date( 'date' , $pattern );
		}
	}

	private function _showOnlyHours() {
		$this->db->where( 'minute(date)', 0 );
		$this->db->where( 'second(date)', 0 );
	}


	function last( $onlyHours = false , $formatDate = array( 'enabled'=> false, 'pattern' => null ) ) {
		if ( $onlyHours ) {
			$this->_showOnlyHours();
		}
		if ( $formatDate['enabled'] ) {
			$pattern = ( isset( $formatDate['pattern'] ) )?$formatDate['pattern'] : null;
			$this->_formatDate( $pattern );
		}else {
			$this->db->select( 'date' );
		}
		$date = new DateTime();
		$dateNow = $date->format( DateTime::ATOM );
		$this->db->where( 'date <= ', $dateNow );
		$this->db->order_by( "id", "desc" );
		$query = $this->db->get( 'records',1);
		return $query->result()[0];
	}

	function get( $onlyHours = false , $formatDate = array( 'enabled'=> false, 'pattern' => null ), $last= '-1 day' , $order = 'asc' ) {
		if ( $last != null ){
		$date = new DateTime();
		$dateNow = $date->format( DateTime::ATOM );
			$date->modify( $last );
		$dateLast = $date->format( DateTime::ATOM  );
		$this->db->where( 'date <= ', $dateNow );
		$this->db->where( 'date >= ', $dateLast );
		}
		if ( $onlyHours ) {
			$this->_showOnlyHours();
		}
		if ( $formatDate['enabled'] ) {
			$pattern = ( isset( $formatDate['pattern'] ) )?$formatDate['pattern'] : null;
			$this->_formatDate( $pattern );
		}else {
			$this->db->select( 'date' );
		}
		$this->db->order_by( "id", $order );
		$query = $this->db->get( 'records' );
		$result = $query->result();
		return $result;
	}

	function fullDays( $onlyHours = false  , $formatDate = array( 'enabled'=> false, 'pattern' => null ), $last= '-0 day' , $order = 'asc'  ) {
		$date = new DateTime();
		if ( $last != null )
			$date->modify( $last );
		$dayStart = $date->format( $this->startOfDayFormat );
		$dayEnd  = $date->format( $this->endOfDayFormat );
		$this->db->where( 'date <= ', $dayEnd );
		$this->db->where( 'date >= ', $dayStart );
		if ( $onlyHours ) {
			$this->_showOnlyHours();
		}
		if ( $formatDate['enabled'] ) {
			$pattern = ( isset( $formatDate['pattern'] ) )?$formatDate['pattern'] : null;
			$this->_formatDate( $pattern );
		}else {
			$this->db->select( 'date' );
		}
		$this->db->order_by( "id", $order );
		$query = $this->db->get( 'records' );
		$result = $query->result();
		return $result;
	}
}
?>
