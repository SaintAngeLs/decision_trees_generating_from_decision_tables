package com.openrules.spatial;

import com.vividsolutions.jts.geom.Geometry;

public class HospitalServiceArea extends GeoEntity {

	private String stateCode = null;
	private String countyCode = null;
	
	public HospitalServiceArea(int i, Geometry geometry) {
		super(i,geometry);
	}

	public String getStateCode() {
		return stateCode;
	}

	public void setStateCode(String stateCode) {
		this.stateCode = stateCode;
	}

	public String getCountyCode() {
		return countyCode;
	}

	public void setCountyCode(String countyCode) {
		this.countyCode = countyCode;
	}

	@Override
	public String getType() {
		return "HSA";
	}

	@Override
	public String toString() {
		return "HospitalServiceArea [stateCode=" + stateCode + ", countyCode="
				+ countyCode + ", getId()=" + getId() + ", getName()="
				+ getName() + ", coordinates="
				+ getGeometry().getCoordinates().length 
				+ ", isSpatial()=" + isSpatial() + "]";
	}

}
