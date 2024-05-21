package imp

import (
	"encoding/csv"
	"log"
	"os"
)

func ReadCsvFile(filePath string) []map[string]string {
	f, err := os.Open(filePath)
	if err != nil {
		log.Fatal("Unable to read input file "+filePath, err)
	}
	defer f.Close()

	csvReader := csv.NewReader(f)
	records, err := csvReader.ReadAll()
	if err != nil {
		log.Fatal("Unable to parse file as CSV for "+filePath, err)
	}

	res := make([]map[string]string, 0)
	attributes := records[0]
	for i, row := range records {
		if i == 0 {
			continue
		}
		m := make(map[string]string)
		for i, value := range row {
			m[attributes[i]] = value
		}
		res = append(res, m)
	}

	return res
}
