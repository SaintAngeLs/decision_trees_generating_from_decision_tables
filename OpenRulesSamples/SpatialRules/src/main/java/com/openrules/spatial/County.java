package com.openrules.spatial;

import com.vividsolutions.jts.geom.Geometry;

public class County extends GeoEntity {
	
	private String stateCode;
	private String countyCode;
	private Integer population;
	
	public County() {
		super();
		stateCode = null;
		countyCode = null;
		population = -1;
	}
	
	public County(int i, Geometry geometry) {
		super(i,geometry);
		stateCode = null;
		countyCode = null;
		population = -1;
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

	public Integer getPopulation() {
		return population;
	}
	public void setPopulation(Integer population) {
		this.population = population;
	}
	
	public String getType() {
		return "County";
	}

	@Override
	public String toString() {
		return "County [stateCode=" + stateCode + ", countyCode=" + countyCode
				+ ", population=" + population + ", getId()=" + getId()
				+ ", getName()=" + getName() + ", getGeometry()="
				+ getGeometry() + ", isSpatial()=" + isSpatial();
	}
	
	
	

}
