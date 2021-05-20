import org.apache.spark.{SparkConf, SparkContext}

object Main extends App {
  
  val conf = new SparkConf()
    .setAppName("Spark")
    .setMaster("local[4]")
    .set("spark.driver.bindAddress", "127.0.0.1")

  val sc = new SparkContext(conf)

  val numSparkErrors = sc
    .textFile("log.txt")
    .filter(_.startsWith("[error]"))
    .filter(_.contains("spark"))
    .count()

  sc.stop()

  println(numSparkErrors)

}
