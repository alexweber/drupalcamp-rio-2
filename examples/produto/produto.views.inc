<?php
/**
 * @file
 * Hooks de Views para o módulo Produto.
 */

/**
 * Controller de Views para a entidade Produto.
 */
class ProdutoViewsController extends EntityDefaultViewsController {

  /**
   * Complementa o hook_views_data().
   */
  public function views_data() {
    $data = parent::views_data();

    // Campo para ver o produto.
    $data['produto']['view'] = array(
      'field' => array(
        'title' => t('Link'),
        'help' => t('Link simples para ver um produto.'),
        'handler' => 'produto_handler_view',
      ),
    );

    // Campo para editar o produto.
    $data['produto']['edit'] = array(
      'field' => array(
        'title' => t('Edit link'),
        'help' => t('Link simples para editar um produto.'),
        'handler' => 'produto_handler_edit',
      ),
    );

    // Campo para excluir o produto.
    $data['produto']['delete'] = array(
      'field' => array(
        'title' => t('Delete link'),
        'help' => t('Link simples para excluir um produto.'),
        'handler' => 'produto_handler_delete',
      ),
    );

    // Add a field for produto operations.
    $data['produto']['operations'] = array(
      'field' => array(
        'title' => t('Operations'),
        'help' => t('Links simples para operações de produtos (ver/editar/excluir).'),
        'handler' => 'produto_handler_operations',
      ),
    );

    return $data;
  }

}