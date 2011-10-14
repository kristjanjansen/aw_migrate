<?php

include_once('aw_migrate_utils.inc');


class AWUserMigration extends Migration {
  public function __construct() {

    parent::__construct();

    $connection = Database::getConnection('default', 'aw');
    
    $query = $connection->select('groups', 'aw_g');
    
    $query->fields('aw_g', array('gid', 'name'))
          ->condition('type', 1)
          ->distinct();
        
    $this->source = new MigrateSourceSQL($query, array(), NULL, array('map_joinable' => FALSE));           

    $this->destination = new MigrateDestinationUser();
    
    $this->map = new MigrateSQLMap($this->machineName,
      array('gid' => array(
              'type' => 'int',
              'unsigned' => TRUE,
              'not null' => TRUE,
              )
      ),
      MigrateDestinationUser::getKeySchema()
    );

    $this->addFieldMapping('name', 'name');
    }

    public function prepareRow($current_row) {
    }

}



class AWImageMigration extends Migration {
  
  public function __construct() {
  
    $this->dependencies = array('AWUser');
    
    parent::__construct();
    
    $connection = Database::getConnection('default', variable_get('aw_migrate_db', 'aw'));
    
    $query = $connection->select('objects', 'aw_o');

    $query->leftJoin('images', 'aw_i', 'aw_o.oid = aw_i.id');

    $query
      ->fields('aw_o', array('oid', 'name', 'createdby', 'created', 'modified', 'status'))
      ->fields('aw_i', array('file'))
      ->condition('aw_o.class_id', 6)
      ->orderBy('aw_o.modified', 'DESC');
             
    $this->source = new MigrateSourceSQL($query, array(), NULL, array('map_joinable' => FALSE));           
    $this->destination = new MigrateDestinationFile(array('copy_file' => TRUE));
 
    $this->map = new MigrateSQLMap($this->machineName,
        array(
          'oid' => array(
            'type' => 'int',
            'unsigned' => TRUE,
            'not null' => TRUE,
          )
        ),
        MigrateDestinationFile::getKeySchema()
      );
  
    $this->addFieldMapping('uid', 'uid')->defaultValue(1);
    $this->addFieldMapping('timestamp', 'created');
    $this->addFieldMapping('changed', 'modified');
    $this->addFieldMapping('uri', 'uri');

    }

  public function prepareRow($current_row) {

    $current_row->uid = _aw_migrate_get_uid($current_row->createdby);
    $current_row->uri = variable_get('aw_domain', '') . '/orb.aw/class=image/action=show/fastcall=1/file=' . basename($current_row->file);
  }

}



class AWFileMigration extends Migration {
  
  public function __construct() {
  
    $this->dependencies = array('AWUser');
    
    parent::__construct();
    
    $connection = Database::getConnection('default', variable_get('aw_migrate_db', 'aw'));
    
    $query = $connection->select('objects', 'aw_o');

    $query->leftJoin('files', 'aw_f', 'aw_o.oid = aw_f.id');

    $query
      ->fields('aw_o', array('oid', 'name', 'createdby', 'created', 'modified', 'status'))
      ->fields('aw_f', array('file'))
      ->condition('aw_o.class_id', 41)
      ->orderBy('aw_o.modified', 'DESC');
             
    $this->source = new MigrateSourceSQL($query, array(), NULL, array('map_joinable' => FALSE));           
    $this->destination = new MigrateDestinationFile(array('copy_file' => TRUE));
 
    $this->map = new MigrateSQLMap($this->machineName,
        array(
          'oid' => array(
            'type' => 'int',
            'unsigned' => TRUE,
            'not null' => TRUE,
          )
        ),
        MigrateDestinationFile::getKeySchema()
      );
  
    $this->addFieldMapping('uid', 'uid')->defaultValue(1);
    $this->addFieldMapping('timestamp', 'created');
    $this->addFieldMapping('changed', 'modified');
    $this->addFieldMapping('uri', 'uri');

    }

  public function prepareRow($current_row) {

    $current_row->uid = _aw_migrate_get_uid($current_row->createdby);
    $current_row->uri = variable_get('aw_domain', '') . '/orb.aw/class=file/action=preview/id='. $current_row->oid . '/' . basename($current_row->file);

}

}




class AWDocMigration extends Migration {
  
  public function __construct() {
  
    $this->dependencies = array('AWUser');
    
    parent::__construct();
    
    $connection = Database::getConnection('default', variable_get('aw_migrate_db', 'aw'));
    
    $query = $connection->select('objects', 'aw_o');
    
    $query->leftJoin('documents', 'aw_d', 'aw_o.oid = aw_d.docid');
    
    $query
      ->fields('aw_o', array('oid', 'name', 'createdby', 'created', 'modified', 'status', 'alias'))
      ->fields('aw_d', array('content'))
      ->condition('aw_o.class_id', 7)
      ->orderBy('aw_o.modified', 'DESC');
   
    $this->source = new MigrateSourceSQL($query, array(), NULL, array('map_joinable' => FALSE));           

    $this->destination = new MigrateDestinationNode('article');
 
    $this->map = new MigrateSQLMap($this->machineName,
      array(
        'oid' => array('type' => 'int',
                      'unsigned' => TRUE,
                      'not null' => TRUE,
                      )
        ),
        MigrateDestinationNode::getKeySchema()
      );
  
    
    $this->addFieldMapping('nid', 'oid');
    $this->addFieldMapping('is_new')->defaultValue(TRUE);
    $this->addFieldMapping('uid', 'uid')->defaultValue(1);
    $this->addFieldMapping('title', 'title');
    $this->addFieldMapping('created', 'created');
    $this->addFieldMapping('changed', 'modified');
    $this->addFieldMapping('path', 'alias');
    $this->addFieldMapping('body', 'body')->arguments(array('format' => 'full_html'));

  }

  public function prepareRow($current_row) {

    $current_row->uid = _aw_migrate_get_uid($current_row->createdby);

    $current_row->title = _aw_migrate_clean_text($current_row->name);
    $current_row->body =  _aw_migrate_clean_text($current_row->content, $current_row->oid); 
   
    // Enable for debugging
    // $current_row->body =  _aw_migrate_clean_text($current_row->content, $current_row->oid). '<br />-------<br />' . $current_row->content;  
  
   }

}