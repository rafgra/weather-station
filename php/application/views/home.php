<?php include 'layout/_header.php'; ?>

<div class="container">
    <fieldset id="data-current">
      <legend>Dane z <span id="date"></span></legend>

      <div class="row">
        <div class="col-md-3">
          <div class="data-container red">
            <div class="placeholder">
              <span id="temperature" class="data">00</span> <span class="unit">&deg;C</span>
            </div>
            <div class="title">
              Temperatura
            </div>
          </div>
        </div>

        <div class="col-md-3">
          <div class="data-container green">
            <div class="placeholder">
              <span id="pressure" class="data">00</span> <span class="unit">hPa</span>
            </div>
            <div class="title">
              Ciśnienie
            </div>
          </div>
        </div>

        <div class="col-md-3">
          <div class="data-container blue">
            <div class="placeholder">
              <span id="humidity" class="data">00</span> <span class="unit">%</span>
            </div>
            <div class="title">
              Wilgotność
            </div>
          </div>
        </div>

        <div class="col-md-3">
          <div class="data-container orange">
            <div class="placeholder">
              <span id="illuminance" class="data">00</span> <span class="unit">lx</span>
            </div>
            <div class="title">
              Jasność
            </div>
          </div>
        </div>
      </div>
    </fieldset>
    <hr />

    <fieldset id="data-24h">
      <legend>Dane z 24 godzin <button type="button" class="btn btn-default btn-xs" data-toggle="collapse" data-target="#collapse-table"><span class="glyphicon glyphicon-resize-full" aria-hidden="true"></span></button></legend>
    <div id="collapse-table" class="collapse">
      <table id="data-table" class="table table-striped header-fixed">
        <thead>
          <tr>
            <th>Data</th>
            <th>Temperatura [&deg;C]</th>
            <th>Ciśnienie [hPa]</th>
            <th>Wilgotność [%]</th>
            <th>Jasność [lx]</th>
            <th>Punkt rosy [&deg;C]</th>
          </tr>
        </thead>
        <tbody>
        </tbody>
      </table>
    </div>
    </fieldset>
    <hr />

    <fieldset id="data-48h">
      <legend>Dane z 48 godzin</legend>

      <div id="chart"></div>
    </fieldset>
    <hr />
  </div><!-- /.container -->
<?php include 'layout/_footer.php'; ?>
