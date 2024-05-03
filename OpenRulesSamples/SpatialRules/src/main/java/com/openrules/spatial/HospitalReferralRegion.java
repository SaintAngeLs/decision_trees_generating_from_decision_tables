package com.openrules.spatial;

import com.vividsolutions.jts.geom.Geometry;

public class HospitalReferralRegion extends GeoEntity {

	private String hrrCode;
	private String stateCode;
	private String countyCode;
	
	public HospitalReferralRegion(int i, Geometry geometry) {
		super(i,geometry);
	}

	public String getHrrCode() {
		return hrrCode;
	}

	public void setHrrCode(String hrrCode) {
		this.hrrCode = hrrCode;
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
		return "HRR";
	}

	@Override
	public String toString() {
		return "HospitalReferralRegion [hrrCode=" + hrrCode + ", stateCode="
				+ stateCode + ", countyCode=" + countyCode + ", getId()="
				+ getId() + ", getName()=" + getName() + ", coordinates="
				+ getGeometry().getCoordinates().length + ", isSpatial()=" + isSpatial() + "]";
	}

}
