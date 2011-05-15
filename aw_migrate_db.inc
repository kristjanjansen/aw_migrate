<?php

class AWDocDBMigration extends Migration {
  public function __construct() {
    
    parent::__construct();
        
    $query = db_select('aw_doc_ct_content', 'a_d_c_c')
             ->fields('a_d_c_c', array('oid', 'title', 'content'));

    $this->source = new MigrateSourceSQL($query);
             
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
    $this->addFieldMapping('title', 'title');
  //  $this->addFieldMapping('body', 'content')->arguments(array('format' => 'full_html'));
    $this->addFieldMapping('body', 'content_filtered')->arguments(array('format' => 'full_html'));

  }

  public function prepareRow($current_row) {

    $current_row->content_filtered =  $current_row->content; 
    
    $preg_replace_map = array(
      '#<span style=\"font-weight: bold;\">(.*)</span>#e' => "('<strong>$1</strong>')",
      '#<!--(.*)-->#e' => '',
      '#<strong>([\s]+)</strong>#e' => '',
      '#<strong>GALLERY</strong>#e' => '',
    );
    
    $current_row->content_filtered = preg_replace(array_keys($preg_replace_map), array_values($preg_replace_map), $current_row->content_filtered);
       
    // References
  
    $current_row->content_filtered = preg_replace('#\#file([0-9]+)\##e', "('[file $1]')", $current_row->content_filtered);

    $current_row->content_filtered = preg_replace('#\#pict([0-9]+)\##e', "('[pict $1]')", $current_row->content_filtered);

    $current_row->content_filtered = preg_replace('#\#minigalerii([0-9]+)\##e', "('[minigalerii $1]')", $current_row->content_filtered);
 
 
 
 
 
   
    $replace_map = array(
      '    ' => ' ',
      '   ' => ' ',
      '  ' => ' ',
      ' ' => ' ',
      '<b>' => '<strong>',
      '</b>' => '</strong>',
      '&nbsp;' => '',
      '<strong></strong>' => '',
      '<p></p>' => '',
      '<p><br />' => '<p>',
      'target="_blank" ' => '',
      '&ndash;' => '-',
      '&rsquo;' => "'",
      '&lsquo;' => "'",
      '&ldquo;' => '"',
      '&rdquo;' => '"',
      '&bdquo;' => '"',
      '&acute;' => '´',
      '&Scaron;' => 'Š',
      '&scaron;' => 'Š',
      '&Otilde' => 'Õ',
      '&otilde' => 'õ',
      '&Auml;' => 'Ä',
      '&auml;' => 'ä',
      '&Ouml;' => 'Ö',
      '&ouml;' => 'Ö',
      '&Uuml;' => 'Ü',
      '&uuml;' => 'ü',
    );
  
    $current_row->content_filtered = str_replace(array_keys($replace_map), array_values($replace_map), $current_row->content_filtered);

    // Omitting html, body, div, span, fieldset, form, input, select, textarea, label, legend

     $allowed_tags = array(
     'applet','object','iframe','h1','h2','h3','h4','h5','h6',
     'p','blockquote','pre','a','abbr','acronym','address','big','cite','code',
     'del','dfn','em','font','img','ins','kbd','q','s','samp','small','strike',
     'strong','sub','sup','tt','var','b','u','i','center','dl','dt','dd','ol',
     'ul','li', 'table','caption','tbody','tfoot','thead','tr','th','td'
     );

     $current_row->content_filtered = filter_xss($current_row->content_filtered, $allowed_tags);

     $files = array();
       $result = db_select('aw_aliases', 'aw_a')
         ->fields('aw_a', array('source', 'target', 'idx'))
         ->condition('source', $current_row->oid)
         ->condition('type', 41)
         ->execute();
  
       if ($result) {
         foreach ($result as $row) {
           
           $query_files = db_select('aw_files', 'aw_f')
              ->fields('aw_f', array('file'))
              ->condition('id', $row->target)
              ->execute();
            
            foreach ($query_files as $query_file) {
              $file = $query_file->file;
            }
           
            $files[] = $row->target.'('.$file.')';
           
         }    
        $current_row->content_filtered .= implode('|', $files);
       }
    
    // Images 
    
    
    $files = array();
      $result = db_select('aw_aliases', 'aw_a')
        ->fields('aw_a', array('source', 'target', 'idx'))
        ->condition('source', $current_row->oid)
        ->condition('type', 6)
        ->execute();
 
      if ($result) {
        foreach ($result as $row) {
          
          $query_files = db_select('aw_images', 'aw_i')
             ->fields('aw_i', array('file'))
             ->condition('id', $row->target)
             ->execute();
           
           foreach ($query_files as $query_file) {
             $file = $query_file->file;
           }
          
           $files[] = $row->target.'['.$file.']';
          
        }    
       $current_row->content_filtered .= implode('|', $files);
      }
    
// INSERT INTO `aliases` (`id`, `source`, `target`, `type`, `data`, `idx`, `relobj_id`, `reltype`, `pri`, `cached`) VALUES
       
       

  }

}