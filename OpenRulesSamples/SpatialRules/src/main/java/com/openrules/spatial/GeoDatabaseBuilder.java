package com.openrules.spatial;

import java.util.List;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import com.vividsolutions.jts.geom.Geometry;

public class GeoDatabaseBuilder {
    
    private static Logger Log = LoggerFactory.getLogger(GeoDatabaseBuilder.class);
	
	public static boolean build() {
		boolean result = true;
		String dataPath = System.getProperty("dataPath");
		if (dataPath == null)
		    dataPath = System.getProperty("user.dir")+"/data";
		List<Geometry> counties = GeoDatabase.getCountyGeometries(dataPath + "/counties_us_gml.gml");
		List<Geometry> hrrs = GeoDatabase.getHRRGeometries(dataPath + "/hrr_gml.gml");
		List<Geometry> hsas = GeoDatabase.getHSAGeometries(dataPath + "/hsa_gml.gml");
//		ReadGML.setPrint(true);
		List<Geometry> hospitals = GeoDatabase.getHospitalGeometries(dataPath + "/hospitals.gml");
		if (counties != null)
			Log.info("There are " + counties.size() + " counties");
		else 
			result = false;
		if (hrrs != null)
			Log.info("There are " + hrrs.size() + " HRRs");
		else 
			result = false;
		if (hsas != null)
			Log.info("There are " + hsas.size() + " HSAs");
		else 
			result = false;
		if (hospitals != null)
			Log.info("There are " + hospitals.size() + " hospitals");
		else 
			result = false;
		return result;
		
	}
	
	public static void main(String[] args) throws Exception {
		if (!GeoDatabaseBuilder.build())
			return;
		int n = 3;
		for (int i = 0; i < n; i++) {
			Geometry county = GeoDatabase.countyGeometries.get(i);
			Log.info("county " + (i+1) + " type=" + county.getGeometryType());
			Log.info(county.toString());
		}
		
		for (int i = 0; i < n; i++) {
			Geometry hrr = GeoDatabase.hrrGeometries.get(i);
			Log.info("hrr " + (i+1) + " type=" + hrr.getGeometryType());
			Log.info(hrr.toString());
		}
		for (int i = 0; i < n; i++) {
			Geometry hsa = GeoDatabase.hsaGeometries.get(i);
			Log.info("hsa " + (i+1) + " type=" + hsa.getGeometryType());
			Log.info(hsa.toString());
		}
		
		if (GeoDatabase.hospitalGeometries.size() > 0) {
			Geometry hospital = GeoDatabase.hospitalGeometries.get(0);
			Log.info(hospital.toString());
		}
	}

}
